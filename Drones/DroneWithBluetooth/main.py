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


def is_number(x):
    try:
        float(x)
        return True
    except ValueError:
        return False

class FirstPage(FloatLayout):
    def __init__(self):
        super().__init__()
        self.page_label = Label(text='roll pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_button = Button(text='change to pitch pid', size_hint=(0.95, 0.1), pos_hint={'center_x':0.5, 'center_y':0.85})
        self.switch_button.bind(on_press=self.switch)
        self.add_widget(self.switch_button)

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

    def switch(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='left')
        myapp.screen_manager.current = 'Second'

    def sendP(self, item):
        if (is_number(self.p_text_input.text)):
            self.p_button.text = 'sent'
            self.p_button.disabled = True
            value = float(self.p_text_input.text)
            ser.write(bytes('a', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.p_button.text = 'No response'
                self.p_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'a':
                    self.p_button.text = 'Wrong response'
                    self.p_button.disabled = False
                else:
                    self.p_button.text = 'Update'
                    self.p_button.disabled = False
            ser.reset_input_buffer()

    def sendI(self, item):
        if (is_number(self.i_text_input.text)):
            self.i_button.text = 'sent'
            self.i_button.disabled = True
            value = float(self.i_text_input.text)
            ser.write(bytes('b', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.i_button.text = 'No response'
                self.i_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'b':
                    self.i_button.text = 'Wrong response'
                    self.i_button.disabled = False
                else:
                    self.i_button.text = 'Update'
                    self.i_button.disabled = False
            ser.reset_input_buffer()

    def sendD(self, item):
        if (is_number(self.d_text_input.text)):
            self.d_button.text = 'sent'
            self.d_button.disabled = True
            value = float(self.d_text_input.text)
            ser.write(bytes('c', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.d_button.text = 'No response'
                self.d_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'c':
                    self.d_button.text = 'Wrong response'
                    self.d_button.disabled = False
                else:
                    self.d_button.text = 'Update'
                    self.d_button.disabled = False
            ser.reset_input_buffer()



class SecondPage(FloatLayout):
    def __init__(self):
        super().__init__()
        self.page_label = Label(text='pitch pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.95})
        self.add_widget(self.page_label)

        self.switch_button = Button(text='change to roll pid', size_hint=(0.95, 0.1), pos_hint={'center_x': 0.5, 'center_y': 0.85})
        self.switch_button.bind(on_press=self.switch)
        self.add_widget(self.switch_button)

        self.p_label = Label(text='P', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.7})
        self.add_widget(self.p_label)
        self.p_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.7},
                                      multiline=False)
        self.add_widget(self.p_text_input)
        self.p_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.7})
        self.p_button.bind(on_press=self.sendP)
        self.add_widget(self.p_button)

        self.i_label = Label(text='I', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.5})
        self.add_widget(self.i_label)
        self.i_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.5},
                                      multiline=False)
        self.add_widget(self.i_text_input)
        self.i_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.5})
        self.i_button.bind(on_press=self.sendI)
        self.add_widget(self.i_button)

        self.d_label = Label(text='D', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.03, 'center_y': 0.3})
        self.add_widget(self.d_label)
        self.d_text_input = TextInput(text='0', size_hint=(0.8, 0.1), pos_hint={'center_x': 0.45, 'center_y': 0.3},
                                      multiline=False)
        self.add_widget(self.d_text_input)
        self.d_button = Button(text='Update', size_hint=(0.1, 0.1), pos_hint={'center_x': 0.92, 'center_y': 0.3})
        self.d_button.bind(on_press=self.sendD)
        self.add_widget(self.d_button)

    def switch(self, item):
        myapp.screen_manager.transition = SlideTransition(direction='right')
        myapp.screen_manager.current = 'First'

    def sendP(self, item):
        if (is_number(self.p_text_input.text)):
            self.p_button.text = 'sent'
            self.p_button.disabled = True
            value = float(self.p_text_input.text)
            ser.write(bytes('d', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.p_button.text = 'No response'
                self.p_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'd':
                    self.p_button.text = 'Wrong response'
                    self.p_button.disabled = False
                else:
                    self.p_button.text = 'Update'
                    self.p_button.disabled = False
            ser.reset_input_buffer()

    def sendI(self, item):
        if (is_number(self.i_text_input.text)):
            self.i_button.text = 'sent'
            self.i_button.disabled = True
            value = float(self.i_text_input.text)
            ser.write(bytes('e', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.i_button.text = 'No response'
                self.i_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'e':
                    self.i_button.text = 'Wrong response'
                    self.i_button.disabled = False
                else:
                    self.i_button.text = 'Update'
                    self.i_button.disabled = False
            ser.reset_input_buffer()

    def sendD(self, item):
        if (is_number(self.d_text_input.text)):
            self.d_button.text = 'sent'
            self.d_button.disabled = True
            value = float(self.d_text_input.text)
            ser.write(bytes('f', 'ascii')+struct.pack('f', value))
            temp = ser.read(1)
            if len(temp) == 0:
                self.d_button.text = 'No response'
                self.d_button.disabled = False
            else:
                temp = struct.unpack('c', temp)[0].decode("ascii")
                if temp != 'f':
                    self.d_button.text = 'Wrong response'
                    self.d_button.disabled = False
                else:
                    self.d_button.text = 'Update'
                    self.d_button.disabled = False
            ser.reset_input_buffer()


class MyApp(App):
    def build(self):
        self.screen_manager = ScreenManager()
        self.first_page = FirstPage()
        screen = Screen(name='First')
        screen.add_widget(self.first_page)
        self.screen_manager.add_widget(screen)
        self.second_page = SecondPage()
        screen = Screen(name='Second')
        screen.add_widget(self.second_page)
        self.screen_manager.add_widget(screen)

        return self.screen_manager


if __name__ == '__main__':
    ports = serial.tools.list_ports.comports()
    bluetooth_port = None
    for port in ports:
        if port.hwid == 'BTHENUM\\{00001101-0000-1000-8000-00805F9B34FB}_LOCALMFG&005D\\7&33E7E024&0&0022110020CF_C00000000':
            bluetooth_port = port
            break
    if bluetooth_port is None:
        exit(-1)
    ser = serial.Serial(bluetooth_port.name, 1382400, timeout=1)
    ser.write(bytes('s', 'ascii'))
    ser.reset_input_buffer()


    myapp = MyApp()
    myapp.run()
    ser.close()