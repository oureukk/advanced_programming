
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
            # 음수인 경우, '-'를 제외한 나머지 부분의 '0'을 제거한 후 '-'와 합칩니다.
            return '-' + num_str[1:].lstrip('0') or '0'
        else:
            # 양수인 경우, 그냥 '0'을 제거합니다.
            return num_str.lstrip('0') or '0'
    i = 0
    j = 0
    count = 0
    for line in lines:

        num1,num2,result,oper=line.split()
        num1=remove_leading_zeros(num1)
        num2=remove_leading_zeros(num2)
        result=remove_leading_zeros(result)

        num1 = int(num1,16)
        num2 = int(num2,16)
        count = count +1
        
        

        if oper == "1":
            compute=remove_hex_prefix(hex(num1 + num2))

            if(result == compute):
                i=i+1
                print("check1",count)
                continue
            else:
                j=j+1
                print("wrong compute1",count)
                print("파이썬 계산\t", compute)
                print("C 계산\t\t", result)

                
        elif oper == "2":
            compute=remove_hex_prefix(hex(num1- num2))


            if(result == compute):
                i=i+1
                print("check2",count)
                continue

            else:
                j=j+1
                print("wrong compute2",count)
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
                print("wrong compute3",count)
                print(f'line : {f.tell()}')
                print("파이썬 계산\t",compute)
                print("C 계산\t\t", result)
                
                
                
        elif oper == "4":
            compute=remove_hex_prefix(hex(int(num1/ num2)))

            
            if(result == compute):   
                i=i+1
                print("check4",count)
                continue
            else:
                j=j+1
                print("wrong compute4",count)
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
