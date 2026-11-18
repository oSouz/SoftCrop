// Original Souz Toolkit effect. Public FFGL SDK supplies only host integration.
#include <FFGLSDK.h>
#include <algorithm>
#include <cmath>
#include "SouzThumbnail.h"
static CFFGLThumbnailInfo souzThumbnail(640,360,souzThumbnailPixels);

class SouzSoftCrop final : public CFFGLPlugin {
public:
    SouzSoftCrop() {
        SetMinInputs(1); SetMaxInputs(1);
        const char* names[] = {"Crop Horizontal", "Crop Vertical",
          "Soft Horizontal", "Soft Vertical", "Corner Blend"};
        for(unsigned i=0;i<5;++i) SetParamInfo(i,names[i],FF_TYPE_STANDARD,0.0f);
    }
    FFResult SetFloatParameter(unsigned i,float v) override {
        if(i>=5 || !std::isfinite(v)) return FF_FAIL;
        values[i]=std::clamp(v,0.0f,1.0f);
        return FF_SUCCESS;
    }
    float GetFloatParameter(unsigned i) override { return i<5 ? values[i] : 0.0f; }
    FFResult InitGL(const FFGLViewportStruct* vp) override;
    FFResult DeInitGL() override { quad.Release(); shader.FreeGLResources(); return FF_SUCCESS; }
    FFResult ProcessOpenGL(ProcessOpenGLStruct* p) override;
private:
    float values[5]{};
    ffglex::FFGLShader shader;
    ffglex::FFGLScreenQuad quad;
};
static CFFGLPluginInfo info(PluginFactory<SouzSoftCrop>,"SZC4","SOUZ-SOFTCROP",
  2,1,1,4,FF_EFFECT,"Symmetric horizontal and vertical crop and feathering",
  "Souz Toolkit - original implementation");
static const char* vertex = R"GLSL(#version 410 core
layout(location=0) in vec4 vPosition;
layout(location=1) in vec2 vUV;
out vec2 uv;
void main(){ gl_Position=vPosition; uv=vUV; }
)GLSL";
static const char* fragment = R"GLSL(#version 410 core
uniform sampler2D image;
uniform vec2 maxUV;
uniform vec4 crop; // left, top, right, bottom
uniform vec4 feather;
uniform float cornerBlend;
in vec2 uv;
out vec4 fragColor;
float edgeMask(float distanceInside, float width){
    // Explicit hard-edge branch avoids smoothstep with equal endpoints.
    if(width<=0.0) return step(0.0,distanceInside);
    return smoothstep(0.0,width,distanceInside);
}
void main(){
    vec4 d=vec4(uv.x,1.0-uv.y,1.0-uv.x,uv.y)-crop;
    vec4 m=vec4(edgeMask(d.x,feather.x),edgeMask(d.y,feather.y),
                edgeMask(d.z,feather.z),edgeMask(d.w,feather.w));
    float productMask=m.x*m.y*m.z*m.w;
    float minimumMask=min(min(m.x,m.y),min(m.z,m.w));
    float mask=mix(productMask,minimumMask,cornerBlend);
    if(crop.x+crop.z>=1.0 || crop.y+crop.w>=1.0) mask=0.0;
    // Scale all channels to preserve premultiplied alpha, including soft edges.
    fragColor=texture(image,uv*maxUV)*mask;
}
)GLSL";
FFResult SouzSoftCrop::InitGL(const FFGLViewportStruct* vp){
    if(!vp) return FF_FAIL;
    if(!shader.Compile(vertex,fragment) || !quad.Initialise()) { DeInitGL(); return FF_FAIL; }
    return CFFGLPlugin::InitGL(vp);
}
FFResult SouzSoftCrop::ProcessOpenGL(ProcessOpenGLStruct* p){
    if(!p || p->numInputTextures<1 || !p->inputTextures || !p->inputTextures[0] || !shader.IsReady()) return FF_FAIL;
    using namespace ffglex;
    ScopedShaderBinding program(shader.GetGLID());
    ScopedSamplerActivation sampler(0);
    Scoped2DTextureBinding texture(p->inputTextures[0]->Handle);
    const auto max=GetMaxGLTexCoords(*p->inputTextures[0]);
    shader.Set("image",0); shader.Set("maxUV",max.s,max.t);
    shader.Set("crop",values[0],values[1],values[0],values[1]);
    shader.Set("feather",values[2],values[3],values[2],values[3]);
    shader.Set("cornerBlend",values[4]); quad.Draw(); return FF_SUCCESS;
}
