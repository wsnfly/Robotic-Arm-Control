import serial
import time

# 配置串口
ser = serial.Serial()
ser.port = 'COM9'  # 串口号
ser.baudrate = 115200  # 波特率
ser.bytesize = 8
ser.stopbits = 1
ser.parity = 'N'


def calc_crc16_modbus(data):
    crc = 0xFFFF
    for byte in data:
        crc ^= byte
        for i in range(8):
            if crc & 0x0001:
                crc = (crc >> 1) ^ 0xA001
            else:
                crc = crc >> 1
    return crc


# 打开串口
ser.open()

# 数组数据
t = 10000
data = [0xcc,0x01,0x03,0x01,0x00,0xff,0x00,0x00,0x00,0x00,0x00,0x00,0x00]
weizhi = 0


# 将数组转换为字节数组
# byte_data = bytearray(data)
cr = calc_crc16_modbus(data)
crc_high_byte = (cr >> 8) & 0xFF
crc_low_byte = cr & 0xFF
data.append(crc_low_byte)
data.append(crc_high_byte)
print(data)
print(1)
# 发送数据到串口

ser.write(data)
time.sleep(0.1)
dat = []
dat = ser.read_all()  # 读取一行数据，以换行符为结束符
print(2)
print(dat)  # 
for i in dat:
    print(i)

# 关闭串口
ser.close()
