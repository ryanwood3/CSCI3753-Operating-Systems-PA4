/*
 * File: pager-predict.c
 * Author:       Andy Sayler
 *               http://www.andysayler.com
 * Adopted From: Dr. Alva Couch
 *               http://www.cs.tufts.edu/~couch/
 *
 * Project: CSCI 3753 Programming Assignment 4
 * Create Date: Unknown
 * Modify Date: 2012/04/03
 * Description:
 * 	This file contains a predictive pageit
 *      implmentation.
 */

#include <stdio.h> 
#include <stdlib.h>

#include "simulator.h"

void pageit(Pentry q[MAXPROCESSES]) { 
    
    /* This file contains the stub for a predictive pager */
    /* You may need to add/remove/modify any part of this file */

    /* Static vars */
    static int program[MAXPROCESSES];
    static int initialized=0;
    static int prevPage[MAXPROCESSES];
    
    /* Local vars */
    int proctmp;
    int pc;
    int page;
    int i;
    int proc;

    /* initialize static vars on first run */
    if(!initialized){
	/* Init complex static vars here */
        for(proctmp=0;proctmp<MAXPROCESSES;proctmp++){
            program[proctmp]=-1;
            prevPage[proctmp]=-1;
        }
	
	initialized = 1;
    }
    
    /* TODO: Implement Predictive Paging */
    for(proc=0; proc<MAXPROCESSES; proc++) { 
    /* Is process active? */
        if(q[proc].active) {
            /* Dedicate all work to first active process*/ 
            int prediction[15]={0};
            pc = q[proc].pc;                // program counter for process
            page = pc/PAGESIZE;         // page the program counter needs
            prediction[page]=1;

            if(page==0 && prevPage[proc]==3){ //single loop=1
                program[proc]=1;
            }
            else if(page==0 && prevPage[proc]==8){ //single loop 8=4
                program[proc]=4;
            }
            else if(page==14){
                program[proc]=3;  //linear=3
            }
            else if(page==9 && prevPage[proc]==13){
                program[proc]=2; //double loop=2
            }
            else if(page==10 && prevPage[proc]==3){
                program[proc]=0; //branch=0
            }
            else if(page==0 && prevPage[proc]==11){
                program[proc]=0;
            }
            else if(program[proc]==1){
                if(page>3){
                    program[proc]=-1;
                }
            }
            else if(program[proc]==4){
                if(page>8){
                    program[proc]=-1;
                }
            }
            else if(program[proc]==3){
                if(page>13){
                    program[proc]=-1;
                }
            }
            else if(program[proc]==0){
                if(page>11){
                    program[proc]=-1;
                }
            }

            if(program[proc]==0){
                if(page==2){
                    prediction[3]=1;
                    prediction[4]=1;
                    prediction[10]=1;
                }
                else if(page==3){
                    prediction[4]=1;
                    prediction[10]=1;
                    prediction[11]=1;
                }
                else if(page==10){
                    prediction[11]=1;
                    prediction[0]=1;
                }
                else if(page==11){
                    prediction[0]=1;
                    prediction[1]=1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            else if(program[proc]==1){
                if(page==2){
                    prediction[3]=1;
                    prediction[0]=1;
                }
                else if(page==3){
                    prediction[0]=1;
                    prediction[1]=1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            else if(program[proc]==2){
                if(page==12){
                    prediction[13]=1;
                    prediction[9]=1;
                }
                else if(page==13){
                    prediction[9]=1;
                    prediction[0]=1;
                    prediction[10]=1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            else if(program[proc]==3){
                if(page==13){
                    prediction[14]=1;
                    prediction[0]=1;
                    program[proc]=-1;
                }
                else if(page==14){
                    prediction[0]=1;
                    program[proc]=-1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            else if(program[proc]==4){
                if(page==7){
                    prediction[8]=1;
                    prediction[0]=1;
                }
                else if(page==8){
                    prediction[0]=1;
                    prediction[1]=1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            else if(program[proc]==-1){
                if(page==2){
                    prediction[3]=1;
                    prediction[4]=1;
                    prediction[10]=1;
                }
                else if(page==3){
                    prediction[4]=1;
                    prediction[10]=1;
                    prediction[11]=1;
                }
                else if(page==7){
                    prediction[8]=1;
                    prediction[0]=1;
                    prediction[9]=1;
                }
                else if(page==8){
                    prediction[0]=1;
                    prediction[9]=1;
                    prediction[10]=1;
                }
                else if(page==10){
                    prediction[11]=1;
                    prediction[0]=1;
                    prediction[12]=1;
                }
                else if(page==11){
                    prediction[0]=1;
                    prediction[12]=1;
                    prediction[13]=1;
                }
                else if(page==12){
                    prediction[13]=1;
                    prediction[0]=1;
                    prediction[14]=1;
                    prediction[9]=1;
                }
                else if(page==13){
                    prediction[0]=1;
                    prediction[14]=1;
                    prediction[9]=1;
                    prediction[10]=1;
                }
                else if(page==14){
                    prediction[0]=1;
                    prediction[1]=1;
                }
                else{
                    prediction[page+1]=1;
                    prediction[page+2]=1;
                }
            }
            for(i = 0; i < 15; i++){
                if(prediction[i] == 0){
                    pageout(proc,i);
                }
            }

            for(i = 0; i < 15; i++){
                if(prediction[i] == 1){
                    pagein(proc, i);
                }
            }
        }
    }
} 
