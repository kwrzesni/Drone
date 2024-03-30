import serial
import serial.tools.list_ports
import matplotlib.pyplot as plt
import matplotlib.animation as animation
import threading
import struct

data_t = []
data_gyro_roll = []
data_gyro_pitch = []
data_gyro_yaw = []
data_acc_x = []
data_acc_y = []
data_acc_z = []
data_roll_angle = []
data_pitch_angle = []
data_altitude = []
data_velocity = []

fig_gyro = plt.figure()
ax_gyro = fig_gyro.add_subplot(1, 1, 1)
fig_acc = plt.figure()
ax_acc = fig_acc.add_subplot(1, 1, 1)
fig_ang = plt.figure()
ax_ang = fig_ang.add_subplot(1, 1, 1)
fig_av = plt.figure()
ax_av = fig_av.add_subplot(1, 1, 1)

ts = []
xs = []
ys = []
zs = []
N = 1000
time_point = 0


def connect(bluetooth_port):
    n = 0
    while True:
        try:
            n += 1
            print('connect try: ', n)
            ser = serial.Serial(bluetooth_port.name, 1382400, timeout=10)
            ser.write(bytes('\0\0\0\0\0', 'ascii'))
            ser.reset_input_buffer()
            return ser
        except serial.SerialException as inst:
            pass


def read_serial(ser):
    global data_t, data_gyro_roll, data_gyro_pitch, data_gyro_yaw, data_acc_x, data_acc_y, data_acc_z, data_roll_angle, data_pitch_angle, data_altitude, data_velocity, time_point
    while True:
        temp = ser.read(4 * 10)
        if len(temp) < 4 * 10:
            ser.write(bytes('\0\0\0\0\0', 'ascii'))
            ser.reset_input_buffer()
            continue
        print('.')
        temp = struct.unpack('f' * 10, temp)
        data_t.append(time_point)
        data_gyro_roll.append(temp[0])
        data_gyro_pitch.append(temp[1])
        data_gyro_yaw.append(temp[2])
        data_acc_x.append(temp[3])
        data_acc_y.append(temp[4])
        data_acc_z.append(temp[5])
        data_roll_angle.append(temp[6])
        data_pitch_angle.append(temp[7])
        data_altitude.append(temp[8])
        data_velocity.append(temp[9])
        time_point += 2.5


def animate_gyroscope(i, ts, xs, ys, zs):
    global data_t, data_gyro_roll, data_gyro_pitch, data_gyro_yaw
    # Add x and y to lists

    # Limit x and y lists to 20 items
    ts = data_t[-N:]
    xs = data_gyro_roll[-N:]
    ys = data_gyro_pitch[-N:]
    zs = data_gyro_yaw[-N:]

    # Draw x and y lists
    ax_gyro.clear()
    ax_gyro.plot(ts, xs)
    ax_gyro.plot(ts, ys)
    ax_gyro.plot(ts, zs)
    ax_gyro.set_ylim(-100, 100)
    ax_gyro.set_title('gyroscope')
    ax_gyro.legend(['roll rate', 'pitch rate', 'yaw rate'])


def animate_accelerometer(i, ts, xs, ys, zs):
    global data_t, data_acc_x, data_acc_y, data_acc_z
    # Add x and y to lists

    # Limit x and y lists to 20 items
    ts = data_t[-N:]
    xs = data_acc_x[-N:]
    ys = data_acc_y[-N:]
    zs = data_acc_z[-N:]

    # Draw x and y lists
    ax_acc.clear()
    ax_acc.plot(ts, xs)
    ax_acc.plot(ts, ys)
    ax_acc.plot(ts, zs)
    ax_acc.set_ylim(-100, 100)
    ax_acc.set_title('accelerometer')
    ax_acc.legend(['x', 'y', 'z'])


def animate_angles(i, ts, xs, ys):
    global data_t, data_roll_angle, data_pitch_angle
    # Add x and y to lists

    # Limit x and y lists to 20 items
    ts = data_t[-N:]
    xs = data_roll_angle[-N:]
    ys = data_pitch_angle[-N:]

    # Draw x and y lists
    ax_ang.clear()
    ax_ang.plot(ts, xs)
    ax_ang.plot(ts, ys)
    ax_ang.set_ylim(-100, 100)
    ax_ang.set_title('angles')
    ax_ang.legend(['roll', 'pitch'])


def animate_altitude_velocity(i, ts, xs, ys):
    global data_altitude, data_velocity
    # Add x and y to lists

    # Limit x and y lists to 20 items
    ts = data_t[-N:]
    xs = data_altitude[-N:]
    ys = data_velocity[-N:]

    # Draw x and y lists
    ax_av.clear()
    ax_av.plot(ts, xs)
    ax_av.plot(ts, ys)
    ax_av.set_ylim(-100, 100)
    ax_av.set_title('altitude, velocity')
    ax_av.legend(['altitude', 'velocity'])
    # Format plot



if __name__ == '__main__':
    ports = serial.tools.list_ports.comports()
    bluetooth_port = None
    for port in ports:
        if port.hwid.find('CF_C00000000') != -1:
            bluetooth_port = port
            break
    if bluetooth_port is None:
        exit(-1)
    ser = connect(bluetooth_port)
    threading.Thread(target=read_serial, args=(ser,)).start()

    ani_gyro = animation.FuncAnimation(fig_gyro, animate_gyroscope, fargs=(ts, xs, ys, zs), interval=100)
    plt.show(block=False)
    ani_acc = animation.FuncAnimation(fig_acc, animate_accelerometer, fargs=(ts, xs, ys, zs), interval=100)
    plt.show(block=False)
    ani_ang = animation.FuncAnimation(fig_ang, animate_angles, fargs=(ts, ys, zs), interval=100)
    plt.show(block=False)
    ani_av = animation.FuncAnimation(fig_av, animate_altitude_velocity, fargs=(ts, ys, zs), interval=100)
    plt.show()
    ser.close()
