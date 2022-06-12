//
// Created by Administrator on 2022/6/8.
//

#include "IResample.h"
#include "XLog.h"
void IResample::update(XData data){
    XData d = this->resample(data);//获得重采样的数据
    if (d.size > 0) {
        LOGE("resample notify");
        this->notify(d);//通知更新
    }
}
void IResample::setName(const char *name) {
    this->name = name;
}