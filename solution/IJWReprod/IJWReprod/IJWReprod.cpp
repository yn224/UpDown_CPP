//===================================================================
// Upcall and Downcall Profiling using IJW Scheme
//===================================================================
// YoungSeok Na

//-------------------------------------------------------------------
// Prelude
//-------------------------------------------------------------------
#include "pch.h"
#include <stdio.h> 
#include <stdlib.h>
#include <vcclr.h>
#include <msclr\marshal.h>
#include <msclr\marshal_windows.h>
#include <msclr\marshal_cppstd.h>
#include <msclr\marshal_atl.h >
#include <iostream> 
#include <chrono>
#include <vector>
#include <list>
#include <array>
#include <cliext/vector>
#include <cliext/list>

using namespace System;
using namespace msclr::interop;

// Measure time using std::chrono
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;

//-------------------------------------------------------------------
// Unmanaged Routines
//-------------------------------------------------------------------
// Unmanaged functions for downcalls
#pragma unmanaged
// Void
void ur_void() {
    return;
}

// Boolean
bool ur_bool() {
    return false;
}

// Signed char
signed char ur_sbyte() {
    return SCHAR_MIN;
}

// Unsigned char
unsigned char ur_byte() {
    return UCHAR_MAX;
}

// Wide char
wchar_t ur_char() {
    return L'•ï';
}

// 16-bit signed 2's C integer
short int ur_int16() {
    return SHRT_MIN;
}

// 16-bit unsigned integer
unsigned short int ur_uint16() {
    return USHRT_MAX;
}

// 32-bit signed 2'sC integer
int ur_int32() {
    return INT_MIN;
}

// 32-bit unsigned integer
unsigned int ur_uint32() {
    return UINT_MAX;
}

// Omitted for comparing consistency
// long int ur_lint32() {
//     return LONG_MIN;
// }
// 
// unsigned long int ur_ulint32() {
//     return ULONG_MAX;
// }

// 64-bit signed 2's C integer
__int64 ur_int64() {
    return -9223372036854775807;
}

// 64-bit unsigned integer
unsigned __int64 ur_uint64() {
    return 18446744073709551615;
}

// Single-precision value
float ur_single() {
    return FLT_MAX_EXP;
}

// Double-precision value
double ur_double() {
    return DBL_MAX;
}

// Long-double integer
long double ur_ldouble() {
    return 3.1415926535897932384626433832795028L;
}

// String
std::string ur_str() {
    return "UR_STR";
}

// Vectors of integers
std::vector<int> ur_vec() {
    std::vector<int> v = { 1, 2, 3, 4, 5 };
    return v;
}

// Tensors
int dim = 10;

std::array<int, 10> ur_tensor1() {
    std::array<int, 10> tensor_d1{};
    for (int i = 0; i < dim; i++) {
        tensor_d1[i] = i;
    }
    return tensor_d1;
}

std::array<std::array<int, 10>, 10> ur_tensor2() {
    std::array<std::array<int, 10>, 10> tensor_d2{};
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            tensor_d2[i][j] = i * dim + j;
        }
    }
    return tensor_d2;
}

std::array<std::array<std::array<int, 10>, 10>, 10> ur_tensor3() {
    std::array<std::array<std::array<int, 10>, 10>, 10> tensor_d3{};
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            for (int k = 0; k < dim; k++) {
                tensor_d3[i][j][k] = i * dim * dim + j * dim + k;
            }
        }
    }
    return tensor_d3;
}

// Structs
struct UStruct {
    int val_int;
    double val_double;
    std::string val_str;
};

UStruct ur_struct() {
    UStruct obj;
    obj.val_int = 32;
    obj.val_double = 3.141592;
    obj.val_str = "UStructObj";
    return obj;
}

// collection of structs(list of structs)
std::list<UStruct> ur_list_struct() {
    std::list<UStruct> lst;
    for (int i = 0; i < 5; i++) {
        UStruct obj;
        obj.val_int = i;
        obj.val_double = 19.314;
        obj.val_str = "hi";
        lst.push_back(obj);
    }
    return lst;
}

//-------------------------------------------------------------------
// Managed Routines
//-------------------------------------------------------------------
// Managed functions for upcalls
#pragma managed
// Void
Void mr_void() {
    return;
}

// Boolean
Boolean mr_bool() {
    return false;
}

// Signed char
SByte mr_sbyte() {
    return SCHAR_MIN;
}

// Unsigned char
Byte mr_byte() {
    return UCHAR_MAX;
}

// Wide character
Char mr_char() {
    return L'•ï';
}

Int16 mr_int16() {
    return SHRT_MIN;
}

UInt16 mr_uint16() {
    return USHRT_MAX;
}

Int32 mr_int32() {
    return INT_MIN;
}

UInt32 mr_uint32() {
    return UINT_MAX;
}

Int64 mr_int64() {
    return -9223372036854775807;
}

UInt64 mr_uint64() {
    return 18446744073709551615;
}

// Float
Single mr_single() {
    return FLT_MAX_EXP;
}

// Double & long-double
Double mr_double() {
    return DBL_MAX;
}

// String
std::string mr_str() {
    String^ str = "Hello";
    return marshal_as<std::string>(str);
}

// Vector
std::vector<int> mr_vec() {
    cliext::vector<Int32> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    v.push_back(5);

    std::vector<int> std_v;
    for each (Int32 val in v)
        std_v.push_back(val);
    return std_v;
}

// Tensor
std::array<int, 10> mr_tensor1() {
    array<Int32>^ mtensor_d1 = gcnew array<Int32>(10);
    for (int i = 0; i < 10; i++) {
        mtensor_d1[i] = i;
    }

    std::array<int, 10> stdarr{};
    for (int i = 0; i < 10; i++) {
        stdarr[i] = mtensor_d1[i];
    }
    return stdarr;
}

std::array<std::array<int, 10>, 10> mr_tensor2() {
    array<array<Int32>^>^ mtensor_d2 = gcnew array<array<Int32>^>(10);
    for (int i = 0; i < 10; i++) {
        mtensor_d2[i] = gcnew array<Int32>(10);
        for (int j = 0; j < 10; j++) {
            mtensor_d2[i][j] = i * 10 + j;
        }
    }

    std::array<std::array<int, 10>, 10> stdarr{};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            stdarr[i][j] = mtensor_d2[i][j];
        }
    }
    return stdarr;
}

std::array<std::array<std::array<int, 10>, 10>, 10> mr_tensor3() {
    array<array<array<Int32>^>^>^ mtensor_d3 = gcnew array<array<array<Int32>^>^>(10);
    for (int i = 0; i < 10; i++) {
        mtensor_d3[i] = gcnew array<array<Int32>^>(10);
        for (int j = 0; j < 10; j++) {
            mtensor_d3[i][j] = gcnew array<Int32>(10);
            for (int k = 0; k < 10; k++) {
                mtensor_d3[i][j][k] = i * 10 * 10 + j * 10 + k;
            }
        }
    }

    std::array<std::array<std::array<int, 10>, 10>, 10> stdarr{};
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            for (int k = 0; k < 10; k++) {
                stdarr[i][j][k] = mtensor_d3[i][j][k];
            }
        }
    }
    return stdarr;
}

// Struct
public ref struct MStruct {
    Int32 val_int;
    Double val_double;
    String^ val_str;
};

UStruct mr_struct() {
    MStruct^ mobj = gcnew MStruct;
    mobj->val_int = 32;
    mobj->val_double = 19.314;
    mobj->val_str = "MObj";

    UStruct uobj;
    uobj.val_int = mobj->val_int;
    uobj.val_double = mobj->val_double;
    String^ str = mobj->val_str;
    uobj.val_str = marshal_as<std::string>(str);
    return uobj;
}

// List of struct
std::list<UStruct> mr_list_struct() {
    cliext::list<MStruct^> lms;
    for (int i = 0; i < 5; i++) {
        MStruct^ mobj = gcnew MStruct;
        mobj->val_int = i;
        mobj->val_double = 19.314;
        mobj->val_str = "hi";
        lms.push_back(mobj);
    }

    std::list<UStruct> lus;
    for each (MStruct^ mst in lms) {
        UStruct ust;
        ust.val_int = mst->val_int;
        ust.val_double = mst->val_double;
        String^ str = mst->val_str;
        ust.val_str = marshal_as<std::string>(str);
        lus.push_back(ust);
    }
    return lus;
}

//-------------------------------------------------------------------
// Unmanaged Realm
//-------------------------------------------------------------------
// Perform and profile varying _upcall_ operations defined above
#pragma unmanaged
double display_time(std::chrono::steady_clock::time_point t1, std::chrono::steady_clock::time_point t2) {
    // If in Linux, use clock_gettime
    duration<double, std::milli> ms_double = t2 - t1;
    double exec_time = ms_double.count();
    return exec_time;
}

void profile_upcall() {
    std::cout << "========================================" << std::endl;
    std::cout << "                 Upcall" << std::endl;
    std::cout << "========================================" << std::endl;
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    double out_time, accum;
    int n_iter = 30;

    // void
    std::cout << "* Upcall void - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        mr_void();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    std::cout << accum << "ms" << std::endl;

    // bool
    std::cout << "* Upcall bool - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        bool b = mr_bool();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Sbyte
    std::cout << "* Upcall sbyte - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        signed char schr = mr_sbyte();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Byte
    std::cout << "* Upcall byte - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        unsigned char uschr = mr_byte();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Char
    std::cout << "* Upcall char - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        wchar_t chr = mr_char();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Int16
    std::cout << "* Upcall int16 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        short int si16 = mr_int16();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // UInt16
    std::cout << "* Upcall uint16 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        unsigned short int usi16 = mr_uint16();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Int32
    std::cout << "* Upcall int32 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        int i32 = mr_int32();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // UInt32
    std::cout << "* Upcall uint32 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        unsigned int ui32 = mr_uint32();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Int64
    std::cout << "* Upcall int64 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        __int64 i64 = mr_int64();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // UInt64
    std::cout << "* Upcall uint64 - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        unsigned __int64 ui64 = mr_uint64();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Single
    std::cout << "* Upcall single - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        float f = mr_single();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Double
    std::cout << "* Upcall double - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        double d = mr_double();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // String
    std::cout << "* Upcall string - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::string str = mr_str();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Vector
    std::cout << "* Upcall vector - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::vector<int> vec = mr_vec();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Tensors
    std::cout << "* Upcall 1D tensor - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::array<int, 10> d1_tensor = mr_tensor1();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    std::cout << "* Upcall 2D tensor - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::array<std::array<int, 10>, 10> d2_tensor = mr_tensor2();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    std::cout << "* Upcall 3D tensor - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::array<std::array<std::array<int, 10>, 10>, 10> d3_tensor = mr_tensor3();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // Struct
    std::cout << "* Upcall struct - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        UStruct ust = mr_struct();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;

    // List struct
    std::cout << "* Upcall list of structs - ";
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        std::list<UStruct> ust = mr_list_struct();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double)n_iter;
    std::cout << accum << "ms" << std::endl;
}

//-------------------------------------------------------------------
// Managed Realm
//-------------------------------------------------------------------
// Perform and profile varying _downcall_ operations defined above
#pragma managed
void profile_downcall() {
    Console::WriteLine("========================================");
    Console::WriteLine("                Downcall");
    Console::WriteLine("========================================");
    auto t1 = high_resolution_clock::now();
    auto t2 = high_resolution_clock::now();
    double out_time, accum;
    int n_iter = 30;

    // Void
    Console::Write("* Downcall void - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        ur_void();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // Boolean
    Console::Write("* Downcall bool - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Boolean b = ur_bool();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // Sbyte
    Console::Write("* Downcall sbyte - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        SByte sb = ur_sbyte();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // byte
    Console::Write("* Downcall byte - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Byte bt = ur_byte();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // char
    Console::Write("* Downcall char - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Char chr = ur_char();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // int16
    Console::Write("* Downcall int16 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Int16 i16 = ur_int16();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // uint16
    Console::Write("* Downcall uint16 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        UInt16 ui16 = ur_uint16();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // int32
    Console::Write("* Downcall int32 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Int32 i32 = ur_int32();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // uint32
    Console::Write("* Downcall uint32 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        UInt32 ui32 = ur_uint32();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // int64
    Console::Write("* Downcall int64 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Int64 i64 = ur_int64();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // uint64
    Console::Write("* Downcall uint64 - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        UInt64 ui64 = ur_uint64();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // single
    Console::Write("* Downcall single - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Single flt = ur_single();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // double
    Console::Write("* Downcall double - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        Double dbl = ur_double();
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // String
    Console::Write("* Downcall string - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        String^ str = gcnew String(ur_str().c_str());
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // Vector
    Console::Write("* Downcall vector - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        cliext::vector<Int32> vec;
        std::vector<int> res = ur_vec();
        for each (int elem in res)
            vec.push_back(elem);
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // Tensors
    Console::Write("* Downcall 1D tensor - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        array<Int32>^ marr = gcnew array<Int32>(10);
        std::array<int, 10> arr1 = ur_tensor1();
        for (int i = 0; i < 10; i++) {
            marr[i] = arr1[i];
        }
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    Console::Write("* Downcall 2D tensor - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        array<array<Int32>^>^ marr = gcnew array<array<Int32>^>(10);
        std::array<std::array<int, 10>, 10> arr2 = ur_tensor2();
        for (int i = 0; i < 10; i++) {
            marr[i] = gcnew array<Int32>(10);
            for (int j = 0; j < 10; j++) {
                marr[i][j] = arr2[i][j];
            }
        }
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    Console::Write("* Downcall 3D tensor - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        array<array<array<Int32>^>^>^ marr = gcnew array<array<array<Int32>^>^>(10);
        std::array<std::array<std::array<int, 10>, 10>, 10> arr3 = ur_tensor3();
        for (int i = 0; i < 10; i++) {
            marr[i] = gcnew array<array<Int32>^>(10);
            for (int j = 0; j < 10; j++) {
                marr[i][j] = gcnew array<Int32>(10);
                for (int k = 0; k < 10; k++) {
                    marr[i][j][k] = arr3[i][j][k];
                }
            }
        }
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // Struct
    Console::Write("* Downcall struct - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        MStruct^ mst = gcnew MStruct();
        UStruct ust = ur_struct();
        mst->val_int = ust.val_int;
        mst->val_double = ust.val_double;
        mst->val_str = gcnew String(ust.val_str.c_str());
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");

    // List of struct
    Console::Write("* Downcall list of structs - ");
    accum = 0.0;
    for (int i = 0; i < n_iter; i++) {
        t1 = high_resolution_clock::now();
        cliext::list<MStruct^> m_lst;
        std::list<UStruct> um_lst = ur_list_struct();
        for each (UStruct st in um_lst) {
            MStruct^ ut = gcnew MStruct;
            ut->val_int = st.val_int;
            ut->val_double = st.val_double;
            ut->val_str = gcnew String(st.val_str.c_str());
            m_lst.push_back(ut);
        }
        t2 = high_resolution_clock::now();
        out_time = display_time(t1, t2);
        accum += out_time;
    }
    accum /= (double) n_iter;
    Console::WriteLine(accum + "ms");
}

//-------------------------------------------------------------------
// Main
//-------------------------------------------------------------------
// C++/CLI object that performs upcalls & downcalls
int main(array<System::String ^> ^args) {
    profile_upcall();
    profile_downcall();
    return 0;
}
