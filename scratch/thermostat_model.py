#! /usr/bin/env python

temp_target = 22

# system parameters
dial = 8

# weather
temp = 15
dial_target = 5
dial_click_temp = .5

# control system
vent_temp = -1
vent_temp_d = 0

true_vent_temp = []

for i in range(0, 600):
  # system model
  true_vent_temp.append((dial - dial_target) * dial_click_temp + temp_target)
  temp = temp + (true_vent_temp[i] - temp) * .008 # .008 vent efficiency

  # control system
  if i == 0:
    vent_temp = true_vent_temp[i]
    vent_temp_lpf = vent_temp
    dial_lpf = dial

  if i >= 5:
    vent_temp = int(true_vent_temp[i - 5]) # sensor delay 5 sec

  # disruption - AC on
  if i == 200:
    dial_target = 10
  
  vent_temp_lpf = vent_temp_lpf + (vent_temp - vent_temp_lpf) * .18
  vent_temp_d = vent_temp_d + (vent_temp - vent_temp_lpf - vent_temp_d) * .2
  vent_temp_predict = (vent_temp + (vent_temp + vent_temp_d) * 5) / 6

  dial = -.5 * (vent_temp_predict - temp_target) + dial_lpf
  dial_lpf = dial_lpf + (dial - dial_lpf) * .08

  if dial < 0: dial = 0
  if dial > 15: dial = 15

  # logging
  print "%d %f %f %f %f %f %d" % (i, true_vent_temp[i], vent_temp, temp, dial_lpf, dial, int(dial))

