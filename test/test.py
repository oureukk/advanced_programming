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


