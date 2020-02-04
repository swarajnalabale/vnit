#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int *data;
    int size;
} bi;

bi *bi_new() {
    bi *a = malloc(sizeof(bi));
    return a;
}

bi *bi_from_string(char *a) {
    bi *b = bi_new();

    int skip = 0;
    while(a[skip] == '0') { skip++; }

    b->size = strlen(a) - skip;

    if(b->size == 0) {
        b->size++;
        b->data = malloc(b->size * sizeof(int));
        b->data[0] = 0;
    } else {
        b->data = malloc(b->size * sizeof(int));
        int i;
        for(i = 0; i < b->size; i++) {
            b->data[i] = a[skip + i] - '0';
        }
    }

    return b;
}

char *bi_to_string(bi *a) {
    char *b = malloc(a->size * sizeof(char));
    int i;
    for(i = 0; i < a->size; i++) {
        b[i] = a->data[i] + '0';
    }
    return b;
}

bi *bi_add(bi *a, bi *b) {
    bi *c = bi_new();
    // max possible size
    c->size = (a->size > b->size ? a->size : b->size) + 1;
    c->data = malloc(c->size * sizeof(int));
    int i = a->size - 1, j = b->size - 1;
    int k = c->size - 1;
    int carry = 0, tmp;

    while(i >= 0 || j >= 0 || carry > 0) {
        if(i >= 0 && j >= 0) {
            tmp = a->data[i] + b->data[j];
        } else if(i >= 0) {
            tmp = a->data[i];
        } else if(j >= 0) {
            tmp = b->data[j];
        } else {
            tmp = 0;
        }
        tmp += carry;
        carry = tmp / 10;
        c->data[k] = tmp % 10;
        i--; j--; k--;
    }

    // this is definitely leaking memory
    if(c->data[0] == 0) { c->size--; c->data++; }
    return c;
}

bi *bi_multiply(bi *a, bi *b) {
    bi *c = bi_new();
    // max size
    c->size = a->size + b->size;
    c->data = malloc(c->size * sizeof(int));
    { int i; for(i = 0; i < c->size; i++) { c->data[i] = 0; } }

    int i = a->size - 1, j = b->size - 1, k = c->size - 1;
    int carry = 0, tmp, push_left = 0;
    while(i >= 0) {
        k = c->size - 1 - push_left++;
        j = b->size - 1;
        while(j >= 0 || carry > 0) {
            if(j >= 0) {
                tmp = a->data[i] * b->data[j];
            } else {
                tmp = 0;
            }
            tmp += carry;
            carry = tmp / 10;
            c->data[k] += tmp % 10;
            carry += c->data[k] / 10;
            c->data[k] = c->data[k] % 10;
            j--; k--;
        }
        i--;
    }

    // Leaking memory for sure
    while(c->data[0] == 0) { c->size--; c->data++; }
    return c;
}
