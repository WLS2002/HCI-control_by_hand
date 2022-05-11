import cv2
import numpy as np
from mediapipe import solutions
from socket import *
import re


def bytes_to_image(width, height, bs):
    return np.frombuffer(bs, dtype=np.uint8).reshape((height, width, 3))


def process_result(results):
    pattern = re.compile(r"x:\s+([\d\\.\-e]*)\s+y:\s+([\d\\.\-e]*)\s+z:\s+([\d\\.\-e]*)")
    if results.multi_hand_landmarks is None:
        return "[]"
    res_list = []
    hands_num = len(results.multi_hand_landmarks)
    for i in range(hands_num):
        hand = results.multi_hand_landmarks[i]
        res = pattern.findall(str(hand))
        tmp = []
        for tup in res:
            tmpmp = list(tup)
            for index in range(3):
                tmpmp[index] = float(tmpmp[index])

            tmp.append(tmpmp)
        res_list.append(tmp)

    return str(res_list)


class Processor(object):
    def __init__(self):
        self.hands = solutions.hands.Hands(static_image_mode=False,
                                           max_num_hands=2,
                                           min_detection_confidence=0.5)

    def image_process(self, img):
        img = cv2.cvtColor(img, cv2.COLOR_BGR2RGB)
        results = self.hands.process(img)
        return process_result(results)


class Communicator(object):
    def __init__(self):
        self.socket = socket(AF_INET, SOCK_STREAM)
        self.socket.connect(("127.0.0.1", 1234))

    def send(self, message: str):
        self.socket.send(message.encode())

    def receive_int(self):
        bb = self.socket.recv(10)
        return int(bb)

    def receive_img(self, width, height):
        bb = self.socket.recv(width * height * 3)
        return bytes_to_image(width, height, bb)

    def close(self):
        self.socket.close()


def main():
    processor = Processor()
    communicator = Communicator()

    width = communicator.receive_int()
    height = communicator.receive_int()

    res = "begin"
    communicator.send(res)

    while True:
        img = communicator.receive_img(width, height)
        res = processor.image_process(img)
        communicator.send(res)

    communicator.close()


if __name__ == '__main__':
    main()
