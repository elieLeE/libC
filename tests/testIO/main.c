#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../shared/ftest.h"
#include "test.h"

int main(){
    affResultTest("testLecture\t\t", testLectureSimple());
    affResultTest("testLectureMatrice\t",testLectureMatrice());
    return 0;
}
