### 21/03/11 (목)
------------------------------
# 1. Markdown
- Typora editor를 사용하면 더 편하게 작업할 수 있다.

- 기본 문법
    1. `#` : 개수(1~6개)에 따라 텍스트 크기 조절 가능
    2. `--------------` : 구분선 긋기
    3. ` ```[텍스트]``` ` : 코드블럭
		
- 이미지 삽입할 때는 상대경로로!
    ```Markdown 
    ![이름]([이미지의 상대경로])
    ```
	

----------------------------
# 2. Ubuntu Terminal Env

- ## vim

    0. vi를 사용하기 편하게 설정파일을 수정한다.

	```
	vi ~/.vimrc
	```
	
	```
	set expandtab
	set ts=4
	set sw=4
	set nobackup
	set autoindent
	
	syn on
	set hlsearch
	set nu
	
	set guifond=Monospace\ 16
	autocmd BufNewFile,BufRead Makefile,makefile set noexpandtab
	
	```

    1. 명령어를 입력할 때는 현재 명령 모드로 있어야함 다른 모드로 있으면 esc를 눌러서 명령 모드로 변환
        ```
        :wq	: 저장 후 나가기
        :sp	: 화면 분할
        Ctrl + w w : 분활 화면 이동

        i	: 입력 모드
        a	: 다음 칸 부터 입력모드
        o	: 다음 행 부터 입력모드
        x	: 한 글자 삭제
        dd  : 한줄 삭제 (Ctrl + x)
        3dd : 세줄 삭제
        yy  : 한줄 복사 (Ctrl + c)
        3yy : 세줄 복사
        p   : 붙여넣기 (Ctrl + v)
        u   : 되돌리기 (Ctrl + z)
        o   : 다음 줄에 커서 이동
        r   : 그 칸 대체입력
        dw  : 한개 단어 지우기

        /[검색어]   : 검색하기
                n   : 다음 검색어  

        Shift + j   : 모아주기

        $   : 행의 끝으로

        :1,$s/[바꾸고 싶은 단어]/[바꿀 단어]/g  : 모두바꾸기
        /c  : 바꾸는거 물어봄
        ```

- ## makefile
        
    - 기본개념
        ```
        Makefile은 컴파일을 편하게 하고, 프로그램 구조를 쉽게 파악하기 위해 사용
    
        Makefile은 탭으로 구분되서 들여쓰기가 중요하다.

        * gcc란?
            Gnu C Compiler의 약자로 C파일을 컴파일해주는 명령
        ```

    - 코드
        ```Makefile
        help:
            @echo "make all"
            @echo "make clean"
            @echo "-----------------"

        all:
            gcc -c main.c
            gcc -c foo.c
            gcc -o main main.o foo.o

        clean:
        ```

        터미널에서 "make [명령어]"를 입력하면 작성된 명령어의 코드가 실행된다.

    - 작성요령
        ```
        1. 각 소스파일을 컴파일하여 Object파일을 생성하는 명령어 작성
            gcc -c foo.c
            gcc -c main.c

        2. 이것들을 링크하여 실행파일을 생성하는 명령어 작성
            gcc -o main main.o foo.o
        ```


        ![파일 구조](./Image/구조.JPG)
		


