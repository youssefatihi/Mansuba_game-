#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ensemble.h"


static void test_is_empty(void){
    struct set se;
    se.size=0;
    assert(is_empty(se)==1);
    se.size=1;
    assert(is_empty(se)==0);
    printf("is_empty passed\n");
}


static void test_set_add_set_find(void){
    struct set se=set_empty();
    int x=1;
    set_add(&se,x);
    assert(se.size==1 && se.t[0]==1);
    int b=3;
    set_add(&se,b);
    assert(se.size==2 && se.t[1]==3);
    printf("set_add passed\n");
    assert(set_find(se,b)==1);
    int tmp=5;
    assert(set_find(se,tmp)==0);
    printf("set_find  and shift_left are passed\n");
    }

static void test_set_remove(void){
    struct set set={{1,2,3},3};
    int a=2;
    int b=0;
    int c=3;
    set_remove(&set,a);
    assert(set.size==2);
    set_remove(&set,c);
     assert(set.size==1);
     set_remove(&set,b);
    assert(set.size==1);
    
    printf("set_remove passed\n");
}

static void test_add_sets(void){
    struct set set1={{1,2,3},3};
    struct set set2={{4,5,6,7},4};
    set1=add_sets(set1,set2);
    for(int i=0;i<7;i++){
        assert(set1.t[i]==i+1);
    } 
    assert(set1.size==7);
    printf("add_sets passed\n");

}


int main(){
    test_is_empty();
    test_set_add_set_find();
    test_set_remove();
    test_add_sets();


}





