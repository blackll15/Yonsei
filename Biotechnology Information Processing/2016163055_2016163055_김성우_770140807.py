#김성우 BTE2703-01 금요일 실습 분반
#2016163055, 점이 주어졌을 경우 그래프를 얻고 오차를 계산하는 프로그램
#2017년 11월 9일 작성 python 3.6.2버젼

#파일 이름이 주어졌을때, x와y좌표의 list를 만드는 함수
def make_list(file_name):
    points=[]
    file=open(file_name)
    for line in file:
        line1=line.split()
        points.append([float(line1[0]),float(line1[1])])
    file.close()
    return points
#함수를 추측한 뒤 x값을 넣어 y값을 얻는 함수
def func(x,points):
    res=0
    for xi,yi in points:
        resi=1
        for i in range(len(points)):
            xj=points[i][0]
            if xi==xj:
                continue
            resi*=(x-xj)/(xi-xj)
        res+=resi*yi
    return res
#계산한 값을 오차를 포함해서 출력하는 함수
def PrintError(x,points):
    print('''x = {0:.6f} \napproximate_y = {1:.16f}, y = {2:.16f}, err = {3:.6e}
'''.format(x,func(x,points),x*math.sin(x),abs(x*math.sin(x)-func(x,points))))
import math;import os
#프로그램 설명
print('===  Polynomial interpolation  ===\n')
#파일 이름을 받음
file_name=input(' Enter the file name of x,y points : ')
while not os.path.exists(file_name):
    print('    !!! File cannot be opened: :',file_name)
    file_name=input(' Enter the file name of x,y points : ')
#무엇을 실행할지 받음
reading=input('''
    1. x points in a file
    2. x points from keybord
 Enter 1or 2 for x points: ''')
while reading!='1' and reading!='2':
    reading=input(' Enter 1or 2 for x points:')
if reading=='1':
    #x좌표가 있는 파일이름을 받음
    xfile_name=input(' Enter the file name of x points : ')
    while not os.path.exists(xfile_name):
        print('    !!! File cannot be opened: :',xfile_name)
        xfile_name=input(' Enter the file name of x points : ')
    xfile=open(xfile_name);print()
    for line in xfile:
        PrintError(float(line),make_list(file_name))
    xfile.close()
#2일 경우 x좌표를 직접 입력
else:
    x=(input('     x : '))
    print()
    PrintError(float(x),make_list(file_name))
