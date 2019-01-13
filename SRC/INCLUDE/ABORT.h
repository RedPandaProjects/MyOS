#ifndef ABORT_H
#define ABORT_H
void abort(const char*name, int id, int a, int b, int c, int d);
#define abort(i,name,id,a,b,c,d) if(i==0)abort(name,id,a,b,c,d);
#endif
