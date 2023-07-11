// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <commdlg.h>
#include <stdio.h>
// C++ 헤더 파일
#include <list>
#include <iterator>
#include <ctime>
// 사용자 헤더 파일
#include "ccircle.h"
#include "crectangle.h"

#define DegreeToRadian(degree) ((degree) * PI / 180)
#define PI 3.141592653589793
#define HALF 16383.5
#define MAX 32767.0
#define NONE 0
#define LEFT 1
#define RIGHT 2
#define TOP 3
#define BOTTOM 4
#define CORNER 5