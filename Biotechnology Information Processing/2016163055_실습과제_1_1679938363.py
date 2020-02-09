
#김성우 BTE2703-01 금요일 실습 분반
#2016163055, n까지의 emrip를 세는 프로그램
#2017년 9월 27일 작성 python 3.6.2버젼
print(''' **********    EMIRP NUMBERS    ***********

    emirp is a nonpalindromic prime number
    whose reversal is also a prime
    ''')
#변수들 미리 설정

count=n=p=0

#n과 p를 입력하는 loop로 잘못 입력하면 다시 문구가 나오도록 설정

while (n<=1 or 10000<=n):
    n=int(input(" Enter n (1 < n < 10000): "))
while (p<=0 or 11<=p):    
    p=int(input(" The number of emirps in a line ( 0 < p < 11): "))
print()

# "=====" 여러개로 장식하는 코드

print('='*(p*7-2))

# n 이하의 수를 넣는 코드
for x in range(1,n+1):
    progress=0
    #x가 소수인지 판별하는 코드
    for y in range(2,x):
        if (x%y==0):
            break
        elif(y==x//2+1):
            progress=1#진행상황 저장
    #소수일 경우 숫자를 뒤집음 
    if(progress==1):
        j=''
        k=x
        length=len(str(x))  
        for y in range(1,length+1):
            i=k%10;k=k//10
            j+=str(i)
    #회문인지 판정하고 진행상황 저장
    if(progress==1 and str(x)!=j):
        progress=2
    #뒤집은게 소수인지 확인
    if(progress==2):
        for y in range(2,int(j)):
            if(int(j)%y==0):
                break
            elif(y==int(j)//2+1):
                count+=1
                #줄맞춤
                print(' '*(4-length),x,end="  ")
                if(count%p==0):
                    print()    
                    
                    
if(count%p!=0):
    print()

print('='*(p*7-2))

#마지막으로  n과 count값을 출력

print('''
 The count of emirps less than {0} is {1}'''.format(n,count))          


