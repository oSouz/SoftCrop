#include "../src/SouzSoftCrop.cpp"
#include <iostream>
#include <limits>
int main(){
    SouzSoftCrop p;
    auto check=[&](bool ok){if(!ok){std::cerr<<"Linked crop regression failed\n";std::exit(1);}};
    for(unsigned i=0;i<4;++i){
        check(p.SetFloatParameter(i,0.25f)==FF_SUCCESS);
        check(p.GetFloatParameter(i)==0.25f && p.GetFloatParameter((i+2)%4)==0.25f);
        check(p.SetFloatParameter(i,0.0f)==FF_SUCCESS);
        check(p.GetFloatParameter((i+2)%4)==0.0f);
    }
    p.SetFloatParameter(4,0.3f); p.SetFloatParameter(6,0.7f);
    check(p.GetFloatParameter(4)==0.3f && p.GetFloatParameter(6)==0.7f);
    p.SetFloatParameter(5,0.2f); p.SetFloatParameter(7,0.8f);
    check(p.GetFloatParameter(5)==0.2f && p.GetFloatParameter(7)==0.8f);
    p.SetFloatParameter(2,2.0f);check(p.GetFloatParameter(0)==1.0f);
    check(p.SetFloatParameter(0,std::numeric_limits<float>::quiet_NaN())==FF_FAIL);
    check(p.GetFloatParameter(0)==1.0f);
    std::cout<<"PASS: bidirectional crop pairs, reset, independent feather, bounds\n";
}
