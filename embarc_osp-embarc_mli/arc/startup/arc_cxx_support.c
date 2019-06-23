/*
 * Copyright (c) 2012-2014 Wind River Systems, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ------------------------------------------
 * Copyright (c) 2017, Synopsys, Inc. All rights reserved.

 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:

 * 1) Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.

 * 2) Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.

 * 3) Neither the name of the Synopsys, Inc., nor the names of its contributors may
 * be used to endorse or promote products derived from this software without
 * specific prior written permission.

 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
--------------------------------------------- */

/**
 * \file
 * \ingroup ARC_HAL_STARTUP
 * \brief cpp support for ARC
 */

#if defined(__GNU__)
/* embARC's GNU C++ support takes reference from Zephyr (cpp_xxx.c) */

/**
 * @file - Constructor module
 * @brief
 * The ctors section contains a list of function pointers that execute the
 * C++ constructors of static global objects. These must be executed before
 * the application's main() routine.
 *
 * NOTE: Not all compilers put those function pointers into the ctors section;
 * some put them into the init_array section instead.
 */

/* What a constructor function pointer looks like */

typedef void (*CtorFuncPtr)(void);

/* Constructor function pointer list is generated by the linker script. */

extern CtorFuncPtr __CTOR_LIST__[];
extern CtorFuncPtr __CTOR_END__[];

/**
 *
 * @brief Invoke all C++ style global object constructors
 *
 * This routine is invoked before the execution of the
 * application's main().
 */
void __do_global_ctors_aux(void)
{
	unsigned int nCtors;

	nCtors = (unsigned int)__CTOR_LIST__[0];

	while (nCtors >= 1) {
		__CTOR_LIST__[nCtors--]();
	}
}

typedef void (*DtorFuncPtr)(void);

extern DtorFuncPtr __DTOR_LIST__[];
extern DtorFuncPtr __DTOR_END__[];

/**
 *
 * @brief Invoke all C++ style global object destructors
 *
 * This routine is invoked after the execution of the
 * application's main().
 */
void __do_global_dtors_aux(void)
{
	unsigned int nDtors;
	unsigned int i;

	nDtors = (unsigned int)__DTOR_LIST__[0];
	i = 1;

	while (i <= nDtors) {
		__DTOR_LIST__[i++]();
	}
}

void *__dso_handle = 0;

/**
 * @brief Register destructor for a global object
 *
 * @param destructor the global object destructor function
 * @param objptr global object pointer
 * @param dso Dynamic Shared Object handle for shared libraries
 *
 * Function does nothing at the moment, assuming the global objects
 * do not need to be deleted
 *
 * @return N/A
 */
int __cxa_atexit(void (*destructor)(void *), void *objptr, void *dso)
{
	return 0;
}

typedef void (*func_ptr)(void);

extern func_ptr __init_array_start[0];
extern func_ptr __init_array_end[0];

/**
 * @brief Execute initialization routines referenced in .init_array section
 *
 * @return N/A
 */
void __do_init_array_aux(void)
{
	for (func_ptr *func = __init_array_start;
		func < __init_array_end;
		func++) {
		(*func)();
	}
}

/**
 * @brief Stub for pure virtual functions
 *
 * This routine is needed for linking C++ code that uses pure virtual
 * functions.
 *
 * @return N/A
 */
void __cxa_pure_virtual(void)
{
	while (1) {
		;
	}
}
#endif