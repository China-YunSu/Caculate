@echo off
cl -c number.c
cl -c myStack.c
cl -c experssion.c
cl -c showError.c

cl calculator.c number.obj myStack.obj experssion.obj showError.obj
