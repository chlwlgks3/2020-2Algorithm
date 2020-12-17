# 2020-2Algorithm  
2020-2 장길진교수님의 알고리즘 강의 정리한것입니다.  

## Hw1  

### hw1-1.c  
(c언어복습)학번 이름 학과 출력하는 C프로그램

### hw1-2.c  
3개의 정수를 command line arguments로 입력받은후 크기 순으로 출력하는 프로그램  

### hw1-3.c  
![image](https://user-images.githubusercontent.com/64358334/102443108-506d0d80-4069-11eb-84c7-99ca91b91622.png)  
![image](https://user-images.githubusercontent.com/64358334/102443136-611d8380-4069-11eb-99e7-da1a5024af65.png)
텍스트 파일을 command line arguments로 입력받은후 최소.최대.평균.분산을 출력해주는 프로그램  

## Hw2  

### hw2-1.c  
![image](https://user-images.githubusercontent.com/64358334/102443324-c5d8de00-4069-11eb-9cc8-d3e2234a2a84.png)  
![image](https://user-images.githubusercontent.com/64358334/102443335-cf624600-4069-11eb-9098-d5aa77bb5eb5.png)

txt파일에서 단어들을 읽어들인후 insertion sort, selection sort, bubble sort를 이용하여 출력파일에 정렬해주는 프로그램  

### hw2-2.c  
hw2-1.c는 고정된 크기의 2차원 배열을 사용하기 때문에 문자열을 저장하기위해 메모리의 낭비가 심하다. 그래서 단어의 숫자가 많아지면 실행이 불가능함. 이 문제를 해결하기위해 동적 메모리할당방법을 이용하여 메모리사용량을 최소화하는 프로그램.  


## Hw3  

### hw3-1.c  
![image](https://user-images.githubusercontent.com/64358334/102443490-37189100-406a-11eb-8d09-00f532efde5e.png)  
![image](https://user-images.githubusercontent.com/64358334/102443506-413a8f80-406a-11eb-9b95-3a0cc6877e1c.png)  

고정된길이의 무작위로 생성된 단어들이 .을 이용하여 구분되어 저장되있다. merge sort와 quick sort이용하여 정렬하여 output파일에 출력하는 프로그램.

### hw3-2.c  
고정된 길이가 아닌 가변길이의 무작위로 생성된 단어들을 mertge sort와 quick sort이용하여 정렬하는 


## Hw4  

### hw4-1.c  
![image](https://user-images.githubusercontent.com/64358334/102443659-9e364580-406a-11eb-942a-ccb3a8180655.png)  
![image](https://user-images.githubusercontent.com/64358334/102443929-40eec400-406b-11eb-860c-960fc4f8f5be.png)
hash table에서 chaining을 이용하여 단어들을 저장하는 프로그램. 각 bucket에 저장될때는 단어의 오름차순으로 유지한다.  
다 읽고 난후에는 table에 저장되있는 전체단어를 오름차순으로 출력하는 프로그램.

### hw4-2.c  
입력파일은 hw4-1.c와 동일하고 chaining을 이용하여 저장하는것도 동일하지만 bucket에 단어 출현횟수 빈도수의 내림차순, 그리고 단어의 alphabetic 오름차순으로 저장되도록한다. 단어가 테이블에 삽입되는 동안에 hash table안에서 이 정렬규칙이 적용되어야함.  
![image](https://user-images.githubusercontent.com/64358334/102443910-346a6b80-406b-11eb-875c-7ab78e554759.png)


## Hw5  

###  hw5.c  
LCS(longest common subsequence) 알고리즘을 구현한 프로그램. 그와 더불어 LCS circular shift도 구현함.

![image](https://user-images.githubusercontent.com/64358334/102444037-83b09c00-406b-11eb-847b-fd1672f42727.png)


