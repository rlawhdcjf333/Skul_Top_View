//미리컴파일된 헤더(Precompiled Header) 는 자주변경되지 않는 긴소스를 미리 컴파일하여 컴파일결과를 별도의 파일에 저장하여,
//다시 똑같은 헤더를 컴파일시 해당파일을 처음부터 컴파일하지않고 미리컴파일된 헤더 파일을 사용해 컴파일 속도를 월등히 향상시켜줍니다

#ifndef PCH_H
#define PCH_H

// 여기에 미리 컴파일하려는 헤더 추가
#include "framework.h"

#endif //PCH_H
