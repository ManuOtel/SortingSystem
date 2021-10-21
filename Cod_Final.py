import cv2
import onnx
from onnx_tf.backend import prepare
import numpy as np
from serial import Serial

cap = cv2.VideoCapture(1, cv2.CAP_DSHOW)
ret, frame = cap.read()
ocr1 = onnx.load('retea92.onnx')
ocr = prepare(ocr1)
with open('Judete.csv', 'r') as file:
    data = file.read()
judete = data.split(",")
letter_string = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
sharpen_filter = np.array([[0, -1, 0], [-1, 5, -1], [0, -1, 0]])
ser = Serial('COM3', 9600)
nevoie_citire = False


while True:
    if nevoie_citire == True:
        ret, frame = cap.read()
        frame = np.asarray(frame, dtype=np.uint8)
        img = frame
        img = cv2.resize(img, None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
        img = cv2.cvtColor(src=img, code=cv2.COLOR_RGB2GRAY)
        img = cv2.bilateralFilter(img, 11, 27, 27)  # Scoate zgomotele
        edge = cv2.Canny(img, 30, 70)
        #cv2.imshow("Cam 01", edge)
        ret, thresh1 = cv2.threshold(edge, 127, 255, 0)
        contours, hierarchy = cv2.findContours(thresh1, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        contours = sorted(contours, key=cv2.contourArea, reverse=True)[:1]
        location = None
        for countour in contours:
            approx = cv2.approxPolyDP(countour, 2, True)
            if len(approx == 4):
                location = approx
        mask = np.zeros(edge.shape, np.uint8)
        new_image = cv2.drawContours(mask, [location], 0, 255, -1)
        new_image = cv2.bitwise_and(img, img, mask=mask)
        (x, y) = np.where(mask == 255)
        (x1, y1) = (np.min(x), np.min(y))
        (x2, y2) = (np.max(x), np.max(y))
        pachet = img[x1:x2 + 1, y1:y2 + 1]
        img = cv2.resize(pachet, None, fx=2, fy=2, interpolation=cv2.INTER_CUBIC)
        img = cv2.filter2D(img, -1, sharpen_filter)
        edge = cv2.Canny(img, 30, 200)
        ret, thresh1 = cv2.threshold(edge, 127, 255, 0)
        contours, hierarchy = cv2.findContours(thresh1, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)
        sorted_contours = sorted(contours, key=lambda ctr: cv2.boundingRect(ctr)[0])
        caractere = []
        if len(sorted_contours) > 0:
            for c in sorted_contours:
                x, y, w, h = cv2.boundingRect(c)
                if 22 < w < 100 and 25 < h < 100:
                    cv2.rectangle(img, (x, y), (x + w, y + h), (155, 155, 155), 1)
                    cropped_image = img[y - 3:y + h + 3, x - 3:x + w + 3]
                    caractere.append(cropped_image)

        caracterer = []
        if len(caractere) > 0:
            for c in caractere:
                if len(c) > 0:
                    try:
                        resized = cv2.resize(c, (24, 24), cv2.INTER_AREA)
                        bordurat = cv2.copyMakeBorder(resized, 2, 2, 2, 2, cv2.BORDER_CONSTANT, value=[255, 255, 255])
                        ret, thresh = cv2.threshold(bordurat, 127, 255, cv2.THRESH_BINARY_INV)
                        caracterer.append(thresh)
                    except Exception as e:
                        ret = None
        citire = ""
        for ctf in caracterer:
            c = np.asarray(ctf, dtype=np.float32)
            output = ocr.run(c)
            prediction = int(np.argmax(np.array(output).squeeze(), axis=0))
            citire = citire + letter_string[prediction]
        #cv2.putText(img, str(len(caractere))+citire, (30, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 255, 255))
        #cv2.imshow("Cam 01", img)
        for i in judete:
            if citire.find(i) != -1:
                if len(i) > 0:
                    print(i)
                    #ser.write(judete.index(i) + 1)
                    stri = str((judete.index(i) + 1))
                    command = stri.encode()
                    ser.write(command)
                    msg = ser.readline()
                    #print(msg)
                    nevoie_citire = False
                    break
    else:
        print("Waiting for reading request")
        msg = ser.readline()
        print("Readig")
        nevoie_citire = True
    if cv2.waitKey(1) == ord('q'):
        break
cap.release()
cv2.destroyAllWindows()
