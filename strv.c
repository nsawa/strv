/*
 *	strv.c
 *
 *	strv�`���̃��[�e�B���e�B�֐�
 *
 *	* Wed Dec 16 00:31:23 JST 2015 Naoyuki Sawa
 *	- 1st �����[�X�B
 *	- ���L�̃��|�W�g���Ɋ܂܂�Ă���Astrv�`���̃��[�e�B���e�B�֐��Ɍ݊��Ȋ֐����������܂����B
 *	  ��https://github.com/haraldh/dracut/blob/master/install/strv.c
 *	  �֐��d�l�͏�L�̃I���W�i���łƓ����ŁA�����͓Ǝ��ł��B
 *	- strv�`���̃��[�e�B���e�B�֐��́A������clipstr.c,clipmisc.c���̒��ɂ����������݂��܂��B
 *	  �����̊֐��Ɠ����@�\�ł���֐��́A�}�N���Ƃ��Ď������鎖�ɂ��܂����B
 */
#include "clip.h"
/*****************************************************************************
 *	
 *****************************************************************************/
//�ψ����̊e������𕡐����āAstrv���쐬����B
//���g�p��
//��char** v = strv_new("123","456","789",NULL);
//��strv_print(v);//123 456 789
//��strv_free(v);
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
//�ψ����̊e������𕡐����āAstrv���쐬����B
#ifndef PIECE
char** strv_new_ap(const char* s, va_list ap) {
	char** v = calloc(1, sizeof(char*));
	if(!v) { DIE(); }	//�������s��
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
//(*pv)�̖����Ƀ|�C���^s��ǉ�����B
//������s�͕�������Ȃ����ɒ��ӂ���B
//�I���W�i���ł̎����̓������s��������������0�ȊO��Ԃ����A�������͕K������(0)��Ԃ��B
//���g�p��
//��char** v = strv_new("123","456","789",NULL);
//��char* s = strdup("ABC");
//��strv_push(&v,s);
//��strv_print(v);//123 456 789 ABC
//��strv_free(v);
#ifndef PIECE
int strv_push(char*** pv, char* s) {
	char** v = *pv;
	int n = strv_length(v);
	v = realloc(v, sizeof(char*) * (n + 2));
	if(!v) { DIE(); }
	v[n + 0] = s;
	v[n + 1] = NULL;
	*pv = v;
	return 0;	//�������͕K������(0)��Ԃ��B
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
//(*pv)�̖����ɕ�����s�𕡐����Ēǉ�����B
//�I���W�i���ł̎����̓������s��������������0�ȊO��Ԃ����A�������͕K������(0)��Ԃ��B
//���g�p��
//��char** v = strv_new("123","456","789",NULL);
//��strv_extend(&v,"ABC");
//��strv_print(v);//123 456 789 ABC
//��strv_free(v);
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
//v�𕡐����A�����ɕ�����s�𕡐����Ēǉ�����B
//v�͕ύX����Ȃ��B
//���g�p��
//��char** v1 = strv_new("123","456","789",NULL);
//��char** v2 = strv_append(v1,"ABC");
//��strv_print(v1);//123 456 789
//��strv_print(v2);//123 456 789 ABC
//��strv_free(v1);
//��strv_free(v2);
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
//v1��v2�̐[���R�s�[��A�������A�V����strv���쐬����B
//v1��v2���ύX����Ȃ��B
//���g�p��
//��char** v1 = strv_new("123","456","789",NULL);
//��char** v2 = strv_new("ABC","DEF","GHI",NULL);
//��char** v3 = strv_merge(v1,v2);
//��strv_print(v1);//123 456 789
//��strv_print(v2);//ABC DEF GHI
//��strv_print(v3);//123 456 789 ABC DEF GHI
//��strv_free(v1);
//��strv_free(v2);
//��strv_free(v3);
#ifndef PIECE
char** strv_merge(char** v1, char** v2) {
	char* s;
	v1 = strv_copy(v1);		//v1�̐[���R�s�[���쐬����B
	while((s = *v2++)) {		//v2�̊e������ɂ��āc
		strv_extend(&v1, s);	//������𕡐����Ēǉ�����B
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
//v�̒��́A������s�Ɠ������e�̕������S�ĊJ������B
//v��ύX���邪�A�k�������̕ύX�ł���A�������̍Ċ��蓖�Ă͍s��Ȃ��B
//�]���āA�|�C���^�͕ω������A�߂�l�͏�Ɉ���v�Ɠ����|�C���^�ł���B
//strv_uniq()�̎������Astrv_remove()���������̍Ċ��蓖�Ă��s��Ȃ����Ɉˑ����Ă���̂ŁAstrv_remove()�̋�����ύX���Ă͂����Ȃ��B
//���g�p��
//��char** v = strv_new("123","456","789","456",NULL);
//��strv_remove(v,"456");
//��strv_print(v);//123 789
//��strv_free(v);
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
	return v;	//����v�����̂܂ܕԂ��B
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
//v�̒��́A�d�����镶�����S�ĊJ������B
//v��ύX���邪�A�k�������̕ύX�ł���A�������̍Ċ��蓖�Ă͍s��Ȃ��B
//�]���āA�|�C���^�͕ω������A�߂�l�͏�Ɉ���v�Ɠ����|�C���^�ł���B
//���g�p��
//��char** v = strv_new("123","456","789","456",NULL);
//��strv_uniq(v);
//��strv_print(v);//123 456 789
//��strv_free(v);
#ifndef PIECE
char** strv_uniq(char** v) {
	char** i = v;
	char* t;
	while((t = *i++)) {
		strv_remove(i, t);
	}
	return v;	//����v�����̂܂ܕԂ��B
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
//v�̒�����A������s�Ɠ������e�̕��������������B
//����������A���̕�����̃|�C���^��Ԃ��B
//������Ȃ���΁ANULL��Ԃ��B
//���g�p��
//��char** v = strv_new("123","456","789",NULL);
//��printf("%s\n",strv_find(v,"456"));//456
//��printf("%s\n",strv_find(v,"567"));//(null)
//��strv_free(v);
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
//v1��v2�̒��ɁA�������e�̕����񂪗L�邩����������B
//�L���1�A�������0��Ԃ��B
//���g�p��
//��char** v1 = strv_new("123","456","789",NULL);
//��char** v2 = strv_new("ABC","DEF","GHI",NULL);
//��char** v3 = strv_new("123","456","GHI",NULL);
//��printf("%d\n",strv_overlap(v1,v2));//0
//��printf("%d\n",strv_overlap(v1,v3));//1
//��strv_free(v1);
//��strv_free(v2);
//��strv_free(v3);
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
//v���\�[�g����B
//v��ύX���邪�A�v�f���͕ω������A�������̍Ċ��蓖�Ă͍s��Ȃ��B
//�]���āA�|�C���^�͕ω������A�߂�l�͏�Ɉ���v�Ɠ����|�C���^�ł���B
//���g�p��
//��char** v = strv_new("ABC","123","DEF","456",NULL);
//��strv_sort(v);
//��strv_print(v);//123 456 ABC DEF
//��strv_free(v);
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
//v�̊e��������A�W���o�͂Ɉ�s���\������B
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
