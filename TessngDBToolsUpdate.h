#ifndef TESSNGDBTOOLSUPDATE_H
#define TESSNGDBTOOLSUPDATE_H
#include "TessngDBToolsUpdateBase.h"
#include "singleholder.h"
class TessngDBToolsUpdate : public TessngDBToolsUpdateBase,public PH::SingleHolder<TessngDBToolsUpdate>
{

protected:
    TessngDBToolsUpdate();

    ~TessngDBToolsUpdate();
private:
    friend class  PH::SingleHolder<TessngDBToolsUpdate>;
    TessngDBToolsUpdate(const TessngDBToolsUpdate&) = delete;
};
#endif // TESSNGDBTOOLSUPDATE_H
