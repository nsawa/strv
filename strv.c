/*
 *	strv.c
 *
 *	strv形式のユーティリティ関数
 *
 *	* Wed Dec 16 00:31:23 JST 2015 Naoyuki Sawa
 *	- 1st リリース。
 *	- 下記のリポジトリに含まれている、strv形式のユーティリティ関数に互換な関数を実装しました。
 *	  ⇒https://github.com/haraldh/dracut/blob/master/install/strv.c
 *	  関数仕様は上記のオリジナル版と同じで、実装は独自です。
 *	- strv形式のユーティリティ関数は、既存のclipstr.c,clipmisc.c等の中にもいくつか存在します。
 *	  既存の関数と同じ機能である関数は、マクロとして実装する事にしました。
 */
#include "clip.h"
/*****************************************************************************
 *	
 *****************************************************************************/
//可変引数の各文字列を複製して、strvを作成する。
//□使用例
//│char** v = strv_new("123","456","789",NULL);
//│strv_print(v);//123 456 789
//│strv_free(v);
#ifndef PIECE
char** strv_new(const char* s, ...) {
	char** v;
	va_list ap;
	va_start(ap, s);
	v = strv_new_ap(s, ap);
	va_end(ap);
	return v;
}
#else //PIECE
char** strv_new(const char* s, ...);
asm("
		.code
		.align		1
		.global		strv_new
strv_new:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//可変引数の各文字列を複製して、strvを作成する。
#ifndef PIECE
char** strv_new_ap(const char* s, va_list ap) {
	char** v = calloc(1, sizeof(char*));
	if(!v) { DIE(); }	//メモリ不足
	while(s) {
		strv_extend(&v, s);
		s = va_arg(ap, const char*);
	}
	return v;
}
#else //PIECE
char** strv_new_ap(const char* s, va_list ap);
asm("
		.code
		.align		1
		.global		strv_new_ap
strv_new_ap:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//(*pv)の末尾にポインタsを追加する。
//文字列sは複製されない事に注意せよ。
//オリジナル版の実装はメモリ不足が生じた時に0以外を返すが、当実装は必ず成功(0)を返す。
//□使用例
//│char** v = strv_new("123","456","789",NULL);
//│char* s = strdup("ABC");
//│strv_push(&v,s);
//│strv_print(v);//123 456 789 ABC
//│strv_free(v);
#ifndef PIECE
int strv_push(char*** pv, char* s) {
	char** v = *pv;
	int n = strv_length(v);
	v = realloc(v, sizeof(char*) * (n + 2));
	if(!v) { DIE(); }
	v[n + 0] = s;
	v[n + 1] = NULL;
	*pv = v;
	return 0;	//当実装は必ず成功(0)を返す。
}
#else //PIECE
int strv_push(char*** pv, char* s);
asm("
		.code
		.align		1
		.global		strv_push
strv_push:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//(*pv)の末尾に文字列sを複製して追加する。
//オリジナル版の実装はメモリ不足が生じた時に0以外を返すが、当実装は必ず成功(0)を返す。
//□使用例
//│char** v = strv_new("123","456","789",NULL);
//│strv_extend(&v,"ABC");
//│strv_print(v);//123 456 789 ABC
//│strv_free(v);
#ifndef PIECE
int strv_extend(char*** pv, const char* s) {
	char* t = strdup(s);
	if(!t) { DIE(); }
	return strv_push(pv, t);
}
#else //PIECE
int strv_extend(char*** pv, const char* s);
asm("
		.code
		.align		1
		.global		strv_extend
strv_extend:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vを複製し、末尾に文字列sを複製して追加する。
//vは変更されない。
//□使用例
//│char** v1 = strv_new("123","456","789",NULL);
//│char** v2 = strv_append(v1,"ABC");
//│strv_print(v1);//123 456 789
//│strv_print(v2);//123 456 789 ABC
//│strv_free(v1);
//│strv_free(v2);
#ifndef PIECE
char** strv_append(char** v, const char* s) {
	v = strv_copy(v);
	strv_extend(&v, s);
	return v;
}
#else //PIECE
char** strv_append(char** v, const char* s);
asm("
		.code
		.align		1
		.global		strv_append
strv_append:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//v1とv2の深いコピーを連結した、新しいstrvを作成する。
//v1もv2も変更されない。
//□使用例
//│char** v1 = strv_new("123","456","789",NULL);
//│char** v2 = strv_new("ABC","DEF","GHI",NULL);
//│char** v3 = strv_merge(v1,v2);
//│strv_print(v1);//123 456 789
//│strv_print(v2);//ABC DEF GHI
//│strv_print(v3);//123 456 789 ABC DEF GHI
//│strv_free(v1);
//│strv_free(v2);
//│strv_free(v3);
#ifndef PIECE
char** strv_merge(char** v1, char** v2) {
	char* s;
	v1 = strv_copy(v1);		//v1の深いコピーを作成する。
	while((s = *v2++)) {		//v2の各文字列について…
		strv_extend(&v1, s);	//文字列を複製して追加する。
	}
	return v1;
}
#else //PIECE
char** strv_merge(char** v1, char** v2);
asm("
		.code
		.align		1
		.global		strv_merge
strv_merge:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vの中の、文字列sと同じ内容の文字列を全て開放する。
//vを変更するが、縮小方向の変更であり、メモリの再割り当ては行わない。
//従って、ポインタは変化せず、戻り値は常に引数vと同じポインタである。
//strv_uniq()の実装が、strv_remove()がメモリの再割り当てを行わない事に依存しているので、strv_remove()の挙動を変更してはいけない。
//□使用例
//│char** v = strv_new("123","456","789","456",NULL);
//│strv_remove(v,"456");
//│strv_print(v);//123 789
//│strv_free(v);
#ifndef PIECE
char** strv_remove(char** v, const char* s) {
	char** i = v;
	char** j = v;
	char* t;
	while((t = *i++)) {
		if(!strcmp(s, t)) {
			free(t);
		} else {
			*j++ = t;
		}
	}
	*j = NULL;
	return v;	//引数vをそのまま返す。
}
#else //PIECE
char** strv_remove(char** v, const char* s);
asm("
		.code
		.align		1
		.global		strv_remove
strv_remove:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vの中の、重複する文字列を全て開放する。
//vを変更するが、縮小方向の変更であり、メモリの再割り当ては行わない。
//従って、ポインタは変化せず、戻り値は常に引数vと同じポインタである。
//□使用例
//│char** v = strv_new("123","456","789","456",NULL);
//│strv_uniq(v);
//│strv_print(v);//123 456 789
//│strv_free(v);
#ifndef PIECE
char** strv_uniq(char** v) {
	char** i = v;
	char* t;
	while((t = *i++)) {
		strv_remove(i, t);
	}
	return v;	//引数vをそのまま返す。
}
#else //PIECE
char** strv_uniq(char** v);
asm("
		.code
		.align		1
		.global		strv_uniq
strv_uniq:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vの中から、文字列sと同じ内容の文字列を検索する。
//見つかったら、その文字列のポインタを返す。
//見つからなければ、NULLを返す。
//□使用例
//│char** v = strv_new("123","456","789",NULL);
//│printf("%s\n",strv_find(v,"456"));//456
//│printf("%s\n",strv_find(v,"567"));//(null)
//│strv_free(v);
#ifndef PIECE
char* strv_find(char** v, const char* s) {
	char* t;
	while((t = *v++)) {
		if(!strcmp(s, t)) { break; }
	}
	return t;
}
#else //PIECE
char* strv_find(char** v, const char* s);
asm("
		.code
		.align		1
		.global		strv_find
strv_find:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//v1とv2の中に、同じ内容の文字列が有るかを検索する。
//有れば1、無ければ0を返す。
//□使用例
//│char** v1 = strv_new("123","456","789",NULL);
//│char** v2 = strv_new("ABC","DEF","GHI",NULL);
//│char** v3 = strv_new("123","456","GHI",NULL);
//│printf("%d\n",strv_overlap(v1,v2));//0
//│printf("%d\n",strv_overlap(v1,v3));//1
//│strv_free(v1);
//│strv_free(v2);
//│strv_free(v3);
#ifndef PIECE
int strv_overlap(char** v1, char** v2) {
	char* s;
	while((s = *v1++)) {
		if(strv_find(v2, s)) { return 1; }
	}
	return 0;
}
#else //PIECE
int strv_overlap(char** v1, char** v2);
asm("
		.code
		.align		1
		.global		strv_overlap
strv_overlap:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vをソートする。
//vを変更するが、要素数は変化せず、メモリの再割り当ては行わない。
//従って、ポインタは変化せず、戻り値は常に引数vと同じポインタである。
//□使用例
//│char** v = strv_new("ABC","123","DEF","456",NULL);
//│strv_sort(v);
//│strv_print(v);//123 456 ABC DEF
//│strv_free(v);
#ifndef PIECE
static int strv_sort_compar(const void* _x, const void* _y) {
	char* x = *(char**)_x;
	char* y = *(char**)_y;
	return strcmp(x, y);
}
char** strv_sort(char** v) {
	qsort(v, strv_length(v), sizeof(char*), strv_sort_compar);
	return v;
}
#else //PIECE
char** strv_sort(char** v);
asm("
		.code
		.align		1
		.global		strv_sort
strv_sort:
");
#endif//PIECE
//-----------------------------------------------------------------------------
//vの各文字列を、標準出力に一行ずつ表示する。
#ifndef PIECE
void strv_print(char** v) {
	char* s;
	while((s = *v++)) {
		puts(s);
	}
}
#else //PIECE
void strv_print(char** v);
asm("
		.code
		.align		1
		.global		strv_print
strv_print:
");
#endif//PIECE
