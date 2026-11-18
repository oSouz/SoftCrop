#include "../src/SouzSoftCrop.cpp"
#include <iostream>
#include <limits>
int main(){
    SouzSoftCrop p;
    auto check=[&](bool ok){if(!ok){std::cerr<<"Crop regression failed\n";std::exit(1);}};
    p.SetFloatParameter(0,0.25f); p.SetFloatParameter(1,0.1f);
    check(p.GetFloatParameter(0)==0.25f && p.GetFloatParameter(1)==0.1f);
    p.SetFloatParameter(0,0.0f);
    check(p.GetFloatParameter(0)==0.0f && p.GetFloatParameter(1)==0.1f);
    p.SetFloatParameter(2,0.3f); p.SetFloatParameter(3,0.7f);
    check(p.GetFloatParameter(2)==0.3f && p.GetFloatParameter(3)==0.7f);
    p.SetFloatParameter(2,0.0f);
    check(p.GetFloatParameter(2)==0.0f && p.GetFloatParameter(3)==0.7f);
    p.SetFloatParameter(0,2.0f);check(p.GetFloatParameter(0)==1.0f);
    check(p.SetFloatParameter(0,std::numeric_limits<float>::quiet_NaN())==FF_FAIL);
    check(p.SetFloatParameter(5,0.0f)==FF_FAIL);
    check(p.GetFloatParameter(0)==1.0f);
    std::cout<<"PASS: independent axes, zero reset, horizontal/vertical feather, bounds\n";
}
