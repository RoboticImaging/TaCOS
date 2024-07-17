"""
Send camera parameters to UE with TCP socket
"""

import numpy

# Send parameters to UE
def send_parameters(parameters, conn, solution_num):
    data = str(parameters[0]) + "," + str(parameters[1]) + "," + str(parameters[2]) + "," + str(parameters[3]) + "," + str(parameters[4]) + "," + str(parameters[5]) + "," + str(solution_num)
    data = str.encode(data)
    conn.sendall(data)

    while True:
        data = conn.recv(1024)

        if data:
            data = data.decode("ascii")
            data = data.split(",")
            data_int = numpy.empty(len(data))

            for i in range(0, len(data)):
                ints = list(data[i])

                if len(ints) > 0:
                    data_int[i] = ord(ints[0])
                else:
                    data_int[i] = 0

            return data_int