#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <ole2.h>

//replace standard memory allocators with the OLE memory allocators
//


extern void * operator new (size_t sz)
{
	void *p;
	LPMALLOC m;

	CoGetMalloc(MEMCTX_TASK, &m);
	p = m->Alloc(sz);
	m->Release();
	return p;
}

extern void operator delete (void *p)
{
	 LPMALLOC m;

	CoGetMalloc(MEMCTX_TASK, &m);
	m->Free(p);
	m->Release();
}

extern "C" void *malloc(size_t i)
{
	void *p;
	LPMALLOC m;

	CoGetMalloc(MEMCTX_TASK, &m);
	p = m->Alloc(i);
	m->Release();
	return p;
}

extern "C" void *realloc(void *q, size_t i)
{
	void *p;
	LPMALLOC m;

	CoGetMalloc(MEMCTX_TASK, &m);
	p = m->Realloc(q, i);
	m->Release();
	return p;
}

extern "C" void *calloc(size_t i, size_t n)
{
	void *retval;
	size_t	size;

	size = i *n;
	retval = malloc(size);
	if (retval)
		memset(retval, 0, size);
	return retval;
}


extern "C" void free(void *x)
{
	LPMALLOC m;

	CoGetMalloc(MEMCTX_TASK, &m);
	m->Free(x);
	m->Release();
}
