#김성우 BTE2703-01 금요일 실습 분반
#2016163055, 스케쥴 관리 프로그램
#2017년 12월 7일 작성 python 3.6.2버젼

#상속해주는 class
class Appointment:
    count=0
    #저장하는 함수
    def __init__(self,year,month,day,detail):
        self.year=year
        self.month=month
        self.day=day
        self.detail=detail
    #클래스 변수에 리스트로 저장하는 함수
    def addapp(cls,year,month,day,detail):
        cls.myappointments.append([detail,year,month,day])
    #날짜가 이후인지 클래스 변수랑 대조해서 TF리스트 만드는 함수
    def is_after(cls,year,month,day):
        after=list()
        for app in cls.myappointments:
            if year*10000+month*100+day>=app[1]*10000+app[2]*100+app[3]:
                after.append(True)
            else:
                after.append(False)
        return after
    #occursOn에서 프린트하는 함수
    def printapp(cls,n,year,month,day):
        print()
        print('  There is {} appointment about {} '.format(cls.name,cls.myappointments[n][0]))
    #외부에서 적절한 값을 입력받는 함수로 밖에 써도 되지만 감점당할까봐 클래스 안에 넣음
    def test(s,b,c):
        while 1:
            try:
                a=int(input('  {}: '.format(s)))
                print()
            except:
                print('  invalid input please try again you should enter number')
                print()
                continue
            if a in range(b,c+1):
                break
            else:
                print('  please enter number {} to {}'.format(b,c))
                print()
        return a
#매달 말일을 출력하는 함수
    def endday(y,m):
        if m==2:
            if y%4==0:
                return 29
            else:
                return 28
        elif m%2 and m<8:
            return 31
        elif m%2-1 and m>=8:
            return 31
        else:
            return 30
            
#상속받은 daily class로 occursOn 제외하고 다 똑같음
class daily(Appointment):
    #클래스 변수를 리스트로 저장
    myappointments=list();name='daily'
    def __init__(self,year,month,day,detail):
        Appointment.__init__(self,year,month,day,detail)
    #매일 일어나는 일이므로 약속일 이후면 프린트함
    def occursOn(cls,year,month,day):
        after=Appointment.is_after(cls,year,month,day)
        for n in range(len(after)):
            if after[n]:
                Appointment.printapp(daily,n,year,month,day)
                Appointment.count+=1
class monthly(Appointment):
    myappointments=list();name='monthly'
    def __init__(self,year,month,day,detail):
        Appointment.__init__(self,year,month,day,detail)
    #약속일 이후인지 판별하고 날짜가 같으면 프린트
    def occursOn(cls,year,month,day):
        after=Appointment.is_after(cls,year,month,day)
        for n in range(len(after)):
            if after[n] and monthly.myappointments[n][3]==day:
                Appointment.printapp(monthly,n,year,month,day)
        #count를 쓰는 이유는 약속이 없으면 없다고 프린트 하기 위함
                Appointment.count+=1
        print()
        print('  there is {} appointment that day'.format(Appointment.count))
        #check 여러번 할 경우 count를 초기화 해야함
        Appointment.count=0
class Onetime(Appointment):
    myappointments=list();name='Onetime'
    def __init__(self,year,month,day,detail):
        Appointment.__init__(self,year,month,day,detail)
    #약속일이랑 같은지만 판별하여 프린트
    def occursOn(cls,year,month,day):
        for n in range(len(Onetime.myappointments)):
            if [Onetime.myappointments[n][0],year,month,day]==Onetime.myappointments[n]:
                Appointment.count+=1
                Appointment.printapp(Onetime,n,year,month,day)


print('**********please add your appointments**********')
while 1:
#사용자가 약속을 더할지 체크할지 정하는 메뉴 화면
    g=input('''+++++++++++++++++++++++++++++++++++++++++++++++++++++
  if you want to add more appointments, enter add
  if you want to check appointments, enter check
  if you want to stop, enter quit
+++++++++++++++++++++++++++++++++++++++++++++++++++++
  Enter: ''')
    if g=='add':
        #약속 저장하는 화면
        a=Appointment.test('please enter beginning year of appointment',1700,3000)
        b=Appointment.test('please enter beginning month of appointment',1,12)
        c=Appointment.test('please enter beginning day of appointment',1,Appointment.endday(a,b))
        e=input('  please enter detail of appointment: ')
        print()
        while 1:
            d=input('  please enter type of appointment ex)onetime,daily,monthly: ')
            if d=='onetime':
                Onetime.addapp(Onetime,a,b,c,e)
                break
            elif d=='monthly':
                monthly.addapp(monthly,a,b,c,e)
                break
            elif d=='daily':
                daily.addapp(daily,a,b,c,e)
                break
            else:
                print('invalid input try again')
                continue
        print()
        print('  The appoinment successfully added to your schedule :)')
        print()
    #약속이 있는지 확인하는 화면
    elif g=='check':
        a=Appointment.test('please enter year you want to know',1700,3000)
        b=Appointment.test('please enter month you want to know',1,12)
        c=Appointment.test('please enter day you want to know',1,Appointment.endday(a,b))
        Onetime.occursOn(Onetime,a,b,c)
        daily.occursOn(daily,a,b,c)
        monthly.occursOn(monthly,a,b,c)
        print()
    elif g=='quit':
        print('  Program ended')
        break
    else: print('  invalid input')
