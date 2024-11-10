f = open("test.txt", 'r')
lines = f.readlines()

def remove_hex_prefix(hex_str):
    # 부호가 있는 경우를 체크
    if hex_str.startswith('-'):
        return '-' + hex_str[3:]  # '-0x'를 '-''로 대체
    elif hex_str.startswith('0x'):
        return hex_str[2:]  # '0x'를 제거
    return hex_str  # 어떤 경우에도 해당하지 않으면 그대로 반환

for line in lines:

    num1,num2,result,oper=line.split()
    num1=num1.lstrip('0')
    num2=num2.lstrip('0')
    result=result.lstrip('0')
    num1 = int(num1,16)
    num2 = int(num2,16)
    
    

    if oper == "1":
        compute=remove_hex_prefix(hex(num1 + num2))
        if(result == compute):
            print("check1")
            continue
        else:
            print("wrong compute1")
            print("원래 값", compute)
            print("계산 값", result)
            f.close()
            exit(1)
            
    elif oper == "2":
        compute=remove_hex_prefix(hex(num1- num2))
        if(result == compute):
            print("check2")
            continue

        else:
            print("wrong compute2")
            print("원래 값",hex( num1- num2))
            print("계산 값", result)
            f.close()
            exit(1)
            
    elif oper == "3":
        compute=remove_hex_prefix(hex(num1* num2))
        if(result == compute):
            print("check3")
            continue
        else:
            print("wrong compute3")
            print(f'line : {f.tell()}')
            print("원래 값",hex( num1* num2))
            print("계산 값", result)
            f.close()
            exit(1)
            
    elif oper == "4":
        compute=remove_hex_prefix(hex(num1/ num2))
        if(result == compute):            
            print("check4")
            continue
        else:
            print("wrong compute4")
            print("원래 값", hex(num1/ num2))
            print("계산 값", (result))
            f.close()
            exit(1)

print("fin")

f.close()

