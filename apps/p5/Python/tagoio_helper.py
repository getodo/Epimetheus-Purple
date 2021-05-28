from math import sqrt, atan, pi
import tago

# Tagoio device
my_device = tago.Device('013dc384-56e6-4dc2-8a86-380701c5ad4e')


def make_accel_dict(acceleration, axis, device):
    data = {
        'variable': f"{axis}_Accel_{device}",
        'unit': 'ms^-2',
        'value': acceleration,
    }
    return data


def make_pitch_dict(pitch, device):
    data = {
        'variable': f"Pitch_{device}",
        'unit': '°',
        'value': pitch,
    }
    return data


def make_roll_dict(roll, device):
    data = {
        'variable': f"Roll_{device}",
        'unit': '°',
        'value': roll,
    }
    return data


def upload_to_dashboard(accelArr1, accelArr2, action):
    pitch1 = atan(accelArr1[0] / sqrt(accelArr1[1] *
                  accelArr1[1] + accelArr1[2] * accelArr1[2])) * 180.0 / pi
    roll1 = atan(accelArr1[1] / sqrt(accelArr1[0] *
                 accelArr1[0] + accelArr1[2] * accelArr1[2])) * 180.0 / pi
    pitch2 = atan(accelArr2[0] / sqrt(accelArr2[1] *
                  accelArr2[1] + accelArr2[2] * accelArr2[2])) * 180.0 / pi
    roll2 = atan(accelArr2[1] / sqrt(accelArr2[0] *
                 accelArr2[0] + accelArr2[2] * accelArr2[2])) * 180.0 / pi
    dictArr = []
    dictArr.append(make_accel_dict(accelArr1[0], 'x', 1))
    dictArr.append(make_accel_dict(accelArr2[0], 'x', 2))
    dictArr.append(make_accel_dict(accelArr1[1], 'y', 1))
    dictArr.append(make_accel_dict(accelArr2[1], 'y', 2))
    dictArr.append(make_accel_dict(accelArr1[2], 'z', 1))
    dictArr.append(make_accel_dict(accelArr2[2], 'z', 2))
    dictArr.append(make_pitch_dict(pitch1, 1))
    dictArr.append(make_pitch_dict(pitch2, 2))
    dictArr.append(make_roll_dict(roll1, 1))
    dictArr.append(make_roll_dict(roll2, 2))
    data = {
        'variable': f"Action",
        'value': action,
    }
    dictArr.append(data)
    result = my_device.insert(dictArr)
    # print(result)
