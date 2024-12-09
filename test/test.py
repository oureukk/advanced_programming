try:
    f = open("test.txt", 'r')
    lines = f.readlines()
    count = []
    def remove_hex_prefix(hex_str):
        # 부호가 있는 경우를 체크
        if hex_str.startswith('-'):
            return '-' + hex_str[3:]  # '-0x'를 '-''로 대체
        elif hex_str.startswith('0x'):
            return hex_str[2:]  # '0x'를 제거
        return hex_str  # 어떤 경우에도 해당하지 않으면 그대로 반환

    def remove_leading_zeros(num_str):
        if num_str.startswith('-'):
            stripped = num_str[1:].lstrip('0')
            return '-0' if stripped == '' else '-' + stripped
        else:
            stripped = num_str.lstrip('0')
            return '0' if stripped == '' else stripped

    i = 0
    j = 0
    count = 0
    for line in lines:

        num1,num2,result,oper=line.split()
        num1= remove_leading_zeros(num1)
        num2= remove_leading_zeros(num2)
        result=remove_leading_zeros(result) 
        num1 = int(num1,16)
        num2 = int(num2,16)
        count = count +1

        if oper == "1":
            compute=remove_hex_prefix(hex(num1 + num2))
            if(result == compute):
                i=i+1
                print("check plus",count)
                continue
            else:
                j=j+1
                print("wrong compute plus",count)
                print("파이썬 계산\t", compute)
                print("C 계산\t\t", result)
        elif oper == "2":
            compute=remove_hex_prefix(hex(num1- num2))
            if(result == compute):
                i=i+1
                continue
            else:
                j=j+1
                print("wrong compute sub",count)
                print("파이썬 계산\t",compute)
                print("C 계산\t\t", result)
        elif oper == "3":
            compute=remove_hex_prefix(hex(num1* num2))
            if(result == compute):
                i=i+1
                print("check3",count)
                continue
            else:
                j=j+1
                print("wrong compute MUL",count)
                print("파이썬 계산\t",compute)
                print("C 계산\t\t", result)  
        elif oper == "4":
            if num1==0 or num2 ==0:
                compute = "0"
            else:
                compute=remove_hex_prefix(hex(int(num1/ num2)))
            if(result == compute):   
                i=i+1
                print("check4",count)
                continue
            else:
                j=j+1
                print("wrong compute DIV",count)
                print("파이썬 계산\t", compute)
                print("C 계산\t\t", (result))
                
 
    print("맞은 숫자",i)
    print("틀린 숫자",j)
    print("fin")
    f.close()

except FileNotFoundError:
    print("파일을 찾을 수 없습니다!")
except IOError:
    print("파일을 읽는 동안 오류가 발생했습니다!")

try:
    f = open("test2.txt", 'r')
    lines = f.readlines()

    def remove_hex_prefix(hex_str):
        if hex_str.startswith('-'):
            return '-' + hex_str[3:]  # '-0x'를 '-'로 대체
        elif hex_str.startswith('0x'):
            return hex_str[2:]  # '0x' 제거
        return hex_str

    def remove_leading_zeros(num_str):
        if num_str.startswith('-'):
            stripped = num_str[1:].lstrip('0')
            return '-0' if stripped == '' else '-' + stripped
        else:
            stripped = num_str.lstrip('0')
            return '0' if stripped == '' else stripped

    def mod_exp(base, exp, mod):
        """빠른 모듈러 지수 계산 (Python으로 LTR/RTL 연산 확인용)."""
        result = 1
        base = base % mod  # base를 모듈러로 나눔
        while exp > 0:
            if exp % 2 == 1:  # 홀수 비트 확인
                result = (result * base) % mod
            base = (base * base) % mod
            exp //= 2
        return result

    i = 0  # 성공 카운트
    j = 0  # 실패 카운트
    count = 0  # 총 테스트 횟수

    for line in lines:
        num1, num2, num3, result, oper = line.split()
        num1 = remove_leading_zeros(num1)
        num2 = remove_leading_zeros(num2)
        num3 = remove_leading_zeros(num3)
        result = remove_leading_zeros(result)

        num1 = int(num1, 16)
        num2 = int(num2, 16)
        num3 = int(num3, 16)
        count += 1

        if oper == "5":  # LTR 연산 검증
            compute = remove_hex_prefix(hex(mod_exp(num1, num2, num3)))
            if result == compute:
                i += 1
                print("check LTR", count)
                continue
            else:
                j += 1
                print("wrong compute LTR", count)
                print("Python 계산\t", compute)
                print("C 계산\t\t", result)

        elif oper == "6":  # RTL 연산 검증
            compute = remove_hex_prefix(hex(mod_exp(num1, num2, num3)))  # 동일한 모듈러 지수 계산
            if result == compute:
                i += 1
                print("check RTL", count)
                continue
            else:
                j += 1
                print("wrong compute RTL", count)
                print("Python 계산\t", compute)
                print("C 계산\t\t", result)

    print("맞은 숫자", i)
    print("틀린 숫자", j)
    print("fin")
    f.close()

except FileNotFoundError:
    print("파일을 찾을 수 없습니다!")
except IOError:
    print("파일을 읽는 동안 오류가 발생했습니다!")
