from kivy.app import App
from kivy.uix.label import Label
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.floatlayout import FloatLayout
from kivy.uix.screenmanager import ScreenManager, Screen, SlideTransition
from kivy.uix.textinput import TextInput
import serial
import serial.tools.list_ports
import struct
import threading


def is_number(x):
    try:
        float(x)
        return True
    except ValueError:
        return False


def connect(app, reconnect_button=None):
    n = 0
    while True:
        try:
            n += 1
            if reconnect_button is not None:
                reconnect_button.text = 'try: ' + str(n)
            app.ser = serial.Serial(bluetooth_port.name, 1382400, timeout=1)
            app.ser.write(bytes('\0\0\0\0\0', 'ascii'))
            app.ser.reset_input_buffer()
            if reconnect_button is not None:
                reconnect_button.text = 'reconnect'
            break
        except serial.SerialException as inst:
            pass


def send(ser, code, text_input, button):
    if is_number(text_input.text):
        button.text = '...'
        button.disabled = True
        value = float(text_input.text)
        ser.write(bytes(code, 'ascii') + struct.pack('f', value))
        temp = myapp.ser.read(1)
        if len(temp) == 0:
            button.text = 'No response'
            button.disabled = False
        else:
            temp = struct.unpack('c', temp)[0].decode("ascii")
            if temp != code:
                button.text = 'Wrong response'
                button.disabled = False
            else:
                button.text = 'Update'
                button.disabled = False
        myapp.ser.reset_input_buffer()
    else:
        button.text = 'Wrong input'


class DronePage(FloatLayout):
    def __init__(self):
        super().__init__()

        self.page_label = Label(text='drone', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_drone_button = Button(text='drone', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.12, 'center_y': 0.85})
        self.switch_drone_button.disabled = True
        self.add_widget(self.switch_drone_button)

        self.switch_roll_button = Button(text='roll', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.37, 'center_y': 0.85})
        self.switch_roll_button.bind(on_press=self.switch_roll)
        self.add_widget(self.switch_roll_button)

        self.switch_pitch_button = Button(text='pitch', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.62, 'center_y': 0.85})
        self.switch_pitch_button.bind(on_press=self.switch_pitch)
        self.add_widget(self.switch_pitch_button)

        self.switch_yaw_button = Button(text='yaw', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.87, 'center_y':0.85})
        self.switch_yaw_button.bind(on_press=self.switch_yaw)
        self.add_widget(self.switch_yaw_button)

        self.vertical_speed_label = Label(text='vertical speed', size_hint=(0.1, 0.1), pos_hint={'center_x':0.08, 'center_y':0.7})
        self.add_widget(self.vertical_speed_label)
        self.vertical_speed_text_input = TextInput(text='0', size_hint=(0.7, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.7}, multiline=False)
        self.add_widget(self.vertical_speed_text_input)
        self.vertical_speed_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.7})
        self.vertical_speed_button.bind(on_press=self.send_vertical_speed)
        self.add_widget(self.vertical_speed_button)

        self.roll_angle_label = Label(text='roll angle', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.08, 'center_y': 0.5})
        self.add_widget(self.roll_angle_label)
        self.roll_angle_text_input = TextInput(text='0', size_hint=(0.7, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.5}, multiline=False)
        self.add_widget(self.roll_angle_text_input)
        self.roll_angle_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.5})
        self.roll_angle_button.bind(on_press=self.send_roll_angle)
        self.add_widget(self.roll_angle_button)

        self.pitch_angle_label = Label(text='pitch angle', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.08, 'center_y': 0.3})
        self.add_widget(self.pitch_angle_label)
        self.pitch_angle_text_input = TextInput(text='0', size_hint=(0.7, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.3}, multiline=False)
        self.add_widget(self.pitch_angle_text_input)
        self.pitch_angle_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.3})
        self.pitch_angle_button.bind(on_press=self.send_pitch_angle)
        self.add_widget(self.pitch_angle_button)

        self.yaw_rate_label = Label(text='yaw rate', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.08, 'center_y': 0.1})
        self.add_widget(self.yaw_rate_label)
        self.yaw_rate_text_input = TextInput(text='0', size_hint=(0.7, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.1}, multiline=False)
        self.add_widget(self.yaw_rate_text_input)
        self.yaw_rate_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.1})
        self.yaw_rate_button.bind(on_press=self.send_yaw_rate)
        self.add_widget(self.yaw_rate_button)

        self.reconnect_button = Button(text='reconnect', size_hint=(0.1, 0.05), pos_hint={'center_x': 0.92, 'center_y': 0.95})
        self.reconnect_button.bind(on_press=self.reconnect)
        self.add_widget(self.reconnect_button)

    def switch_roll(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Roll'

    def switch_pitch(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Pitch'

    def switch_yaw(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Yaw'

    def send_vertical_speed(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'j', self.vertical_speed_text_input, self.vertical_speed_button)).start()

    def send_roll_angle(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'k', self.roll_angle_text_input, self.roll_angle_button)).start()

    def send_pitch_angle(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'l', self.pitch_angle_text_input, self.pitch_angle_button)).start()

    def send_yaw_rate(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'm', self.yaw_rate_text_input, self.yaw_rate_button)).start()

    def reconnect(self, item):
        myapp.ser.close()
        threading.Thread(target=connect, args=(myapp, item)).start()


class RollPage(FloatLayout):
    def __init__(self):
        super().__init__()

        self.page_label = Label(text='roll pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_drone_button = Button(text='drone', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.12, 'center_y': 0.85})
        self.switch_drone_button.bind(on_press=self.switch_drone)
        self.add_widget(self.switch_drone_button)

        self.switch_roll_button = Button(text='roll', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.37, 'center_y': 0.85})
        self.switch_roll_button.disabled = True
        self.add_widget(self.switch_roll_button)

        self.switch_pitch_button = Button(text='pitch', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.62, 'center_y': 0.85})
        self.switch_pitch_button.bind(on_press=self.switch_pitch)
        self.add_widget(self.switch_pitch_button)

        self.switch_yaw_button = Button(text='yaw', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.87, 'center_y':0.85})
        self.switch_yaw_button.bind(on_press=self.switch_yaw)
        self.add_widget(self.switch_yaw_button)

        self.p_label = Label(text='P', size_hint=(0.1, 0.1), pos_hint={'center_x':0.03, 'center_y':0.7})
        self.add_widget(self.p_label)
        self.p_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.7}, multiline=False)
        self.add_widget(self.p_text_input)
        self.p_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.7})
        self.p_button.bind(on_press=self.sendP)
        self.add_widget(self.p_button)

        self.i_label = Label(text='I', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.5})
        self.add_widget(self.i_label)
        self.i_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.5}, multiline=False)
        self.add_widget(self.i_text_input)
        self.i_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.5})
        self.i_button.bind(on_press=self.sendI)
        self.add_widget(self.i_button)

        self.d_label = Label(text='D', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.3})
        self.add_widget(self.d_label)
        self.d_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.3}, multiline=False)
        self.add_widget(self.d_text_input)
        self.d_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.3})
        self.d_button.bind(on_press=self.sendD)
        self.add_widget(self.d_button)

        self.reconnect_button = Button(text='reconnect', size_hint=(0.1, 0.05), pos_hint={'center_x': 0.92, 'center_y': 0.95})
        self.reconnect_button.bind(on_press=self.reconnect)
        self.add_widget(self.reconnect_button)

    def switch_drone(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Drone'

    def switch_pitch(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Pitch'

    def switch_yaw(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Yaw'

    def sendP(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'a', self.p_text_input, self.p_button)).start()

    def sendI(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'b', self.i_text_input, self.i_button)).start()

    def sendD(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'c', self.d_text_input, self.d_button)).start()

    def reconnect(self, item):
        myapp.ser.close()
        threading.Thread(target=connect, args=(myapp, item)).start()


class PitchPage(FloatLayout):
    def __init__(self):
        super().__init__()
        self.page_label = Label(text='pitch pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_drone_button = Button(text='drone', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.12, 'center_y': 0.85})
        self.switch_drone_button.bind(on_press=self.switch_drone)
        self.add_widget(self.switch_drone_button)

        self.switch_roll_button = Button(text='roll', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.37, 'center_y': 0.85})
        self.switch_roll_button.bind(on_press=self.switch_roll)
        self.add_widget(self.switch_roll_button)

        self.switch_pitch_button = Button(text='pitch', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.62, 'center_y': 0.85})
        self.switch_pitch_button.disabled = True
        self.add_widget(self.switch_pitch_button)

        self.switch_yaw_button = Button(text='yaw', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.87, 'center_y':0.85})
        self.switch_yaw_button.bind(on_press=self.switch_yaw)
        self.add_widget(self.switch_yaw_button)

        self.p_label = Label(text='P', size_hint=(0.1, 0.1), pos_hint={'center_x':0.03, 'center_y':0.7})
        self.add_widget(self.p_label)
        self.p_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.7}, multiline=False)
        self.add_widget(self.p_text_input)
        self.p_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.7})
        self.p_button.bind(on_press=self.sendP)
        self.add_widget(self.p_button)

        self.i_label = Label(text='I', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.5})
        self.add_widget(self.i_label)
        self.i_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.5}, multiline=False)
        self.add_widget(self.i_text_input)
        self.i_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.5})
        self.i_button.bind(on_press=self.sendI)
        self.add_widget(self.i_button)

        self.d_label = Label(text='D', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.3})
        self.add_widget(self.d_label)
        self.d_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.3}, multiline=False)
        self.add_widget(self.d_text_input)
        self.d_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.3})
        self.d_button.bind(on_press=self.sendD)
        self.add_widget(self.d_button)

        self.reconnect_button = Button(text='reconnect', size_hint=(0.1, 0.05), pos_hint={'center_x': 0.92, 'center_y': 0.95})
        self.reconnect_button.bind(on_press=self.reconnect)
        self.add_widget(self.reconnect_button)

    def switch_drone(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Drone'

    def switch_roll(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Roll'

    def switch_yaw(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Yaw'

    def sendP(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'd', self.d_text_input, self.d_button)).start()

    def sendI(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'e', self.d_text_input, self.d_button)).start()

    def sendD(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'f', self.d_text_input, self.d_button)).start()

    def reconnect(self, item):
        myapp.ser.close()
        threading.Thread(target=connect, args=(myapp, item)).start()

class YawPage(FloatLayout):
    def __init__(self):
        super().__init__()
        self.page_label = Label(text='yaw pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_drone_button = Button(text='drone', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.12, 'center_y': 0.85})
        self.switch_drone_button.bind(on_press=self.switch_drone)
        self.add_widget(self.switch_drone_button)

        self.switch_roll_button = Button(text='roll', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.37, 'center_y': 0.85})
        self.switch_roll_button.bind(on_press=self.switch_roll)
        self.add_widget(self.switch_roll_button)

        self.switch_pitch_button = Button(text='pitch', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.62, 'center_y': 0.85})
        self.switch_pitch_button.bind(on_press=self.switch_pitch)
        self.add_widget(self.switch_pitch_button)

        self.switch_yaw_button = Button(text='yaw', size_hint=(0.2, 0.1), pos_hint={'center_x': 0.87, 'center_y': 0.85})
        self.switch_yaw_button.disabled = True
        self.add_widget(self.switch_yaw_button)

        self.p_label = Label(text='P', size_hint=(0.1, 0.1), pos_hint={'center_x':0.03, 'center_y':0.7})
        self.add_widget(self.p_label)
        self.p_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.7}, multiline=False)
        self.add_widget(self.p_text_input)
        self.p_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.7})
        self.p_button.bind(on_press=self.sendP)
        self.add_widget(self.p_button)

        self.i_label = Label(text='I', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.5})
        self.add_widget(self.i_label)
        self.i_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.5}, multiline=False)
        self.add_widget(self.i_text_input)
        self.i_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.5})
        self.i_button.bind(on_press=self.sendI)
        self.add_widget(self.i_button)

        self.d_label = Label(text='D', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.3})
        self.add_widget(self.d_label)
        self.d_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.3}, multiline=False)
        self.add_widget(self.d_text_input)
        self.d_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.3})
        self.d_button.bind(on_press=self.sendD)
        self.add_widget(self.d_button)

        self.reconnect_button = Button(text='reconnect', size_hint=(0.1, 0.05), pos_hint={'center_x': 0.92, 'center_y': 0.95})
        self.reconnect_button.bind(on_press=self.reconnect)
        self.add_widget(self.reconnect_button)

    def switch_drone(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Drone'

    def switch_roll(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Roll'

    def switch_pitch(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Pitch'

    def sendP(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'g', self.d_text_input, self.d_button)).start()

    def sendI(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'h', self.d_text_input, self.d_button)).start()

    def sendD(self, item):
        threading.Thread(target=send,
                         args=(myapp.ser, 'i', self.d_text_input, self.d_button)).start()

    def reconnect(self, item):
        myapp.ser.close()
        threading.Thread(target=connect, args=(myapp, item)).start()


class MyApp(App):
    def __init__(self):
        super().__init__()
        self.ser = None
        connect(self)

    def build(self):
        self.screen_manager = ScreenManager()
        self.drone_page = DronePage()
        screen = Screen(name='Drone')
        screen.add_widget(self.drone_page)
        self.screen_manager.add_widget(screen)
        self.roll_page = RollPage()
        screen = Screen(name='Roll')
        screen.add_widget(self.roll_page)
        self.screen_manager.add_widget(screen)
        self.pitch_page = PitchPage()
        screen = Screen(name='Pitch')
        screen.add_widget(self.pitch_page)
        self.screen_manager.add_widget(screen)
        self.yaw_page = YawPage()
        screen = Screen(name='Yaw')
        screen.add_widget(self.yaw_page)
        self.screen_manager.add_widget(screen)

        return self.screen_manager


if __name__ == '__main__':
    bluetooth_port = ''
    ports = serial.tools.list_ports.comports()
    bluetooth_port = None
    for port in ports:
        if port.hwid.find('CF_C00000000') != -1:
            bluetooth_port = port
            break
    if bluetooth_port is None:
        exit(-1)
    myapp = MyApp()
    myapp.run()
    myapp.ser.close()