// Original Souz Toolkit effect. Public FFGL SDK supplies only host integration.
#include <FFGLSDK.h>
#include <algorithm>
#include <cmath>
#include "SouzThumbnail.h"
static CFFGLThumbnailInfo souzThumbnail(160,120,souzThumbnailPixels);

class SouzSoftCrop final : public CFFGLPlugin {
public:
    SouzSoftCrop() {
        SetMinInputs(1); SetMaxInputs(1);
        const char* names[] = {"Crop Left", "Crop Top", "Crop Right", "Crop Bottom",
          "Soft Left", "Soft Top", "Soft Right", "Soft Bottom", "Corner Blend"};
        for(unsigned i=0;i<9;++i) SetParamInfo(i,names[i],FF_TYPE_STANDARD,0.0f);
    }
    FFResult SetFloatParameter(unsigned i,float v) override {
        if(i>=9 || !std::isfinite(v)) return FF_FAIL;
        values[i]=std::clamp(v,0.0f,1.0f);
        // The dry crop controls form symmetric horizontal and vertical pairs.
        // Feather values deliberately remain independent.
        if(i<4) values[(i+2)%4]=values[i];
        return FF_SUCCESS;
    }
    float GetFloatParameter(unsigned i) override { return i<9 ? values[i] : 0.0f; }
    FFResult InitGL(const FFGLViewportStruct* vp) override;
    FFResult DeInitGL() override { quad.Release(); shader.FreeGLResources(); return FF_SUCCESS; }
    FFResult ProcessOpenGL(ProcessOpenGLStruct* p) override;
private:
    float values[9]{};
    ffglex::FFGLShader shader;
    ffglex::FFGLScreenQuad quad;
};
static CFFGLPluginInfo info(PluginFactory<SouzSoftCrop>,"SZC1","Souz Soft Crop",
  2,1,1,1,FF_EFFECT,"Linked symmetric crop and independent inward feathering",
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
    shader.Set("crop",values[0],values[1],values[2],values[3]);
    shader.Set("feather",values[4],values[5],values[6],values[7]);
    shader.Set("cornerBlend",values[8]); quad.Draw(); return FF_SUCCESS;
}
