import socket

# Define the IP address and port of the Arduino robot car
robot_ip = '192.168.0.48'  # Change this to the IP address of your robot (you can find it from the Arduino's Serial Monitor)
robot_port = 8888         # The port number the Arduino is listening to (defined as localPort in the Arduino code)

# Create a UDP socket
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

# Function to send a command to the robot
def send_command(command):
    print(f"Sending command: {command}")
    sock.sendto(command.encode(), (robot_ip, robot_port))

# # Control commands
# def go_forward():
#     send_command('A')  # Command 'A' will make the robot move forward 

# def go_left():
#     send_command('B')  # Command 'L' will make the robot turn left

# def go_right():
#     send_command('C')  # Command 'R' will make the robot turn right

# def go_back():
#     send_command('D')  # Command 'B' will make the robot reverse

# def stop_robot():
#     send_command('E')  # Command 'E' will stop the robot

# def obstacle_avoidance():
#     send_command('F')  # Command 'O' will start obstacle avoidance mode

# def line_follow():
#     send_command('G')  # Command 'T' will start line following mode

# Main program loop to control the robot
if __name__ == "__main__":
    while True:
        print("Enter a command to control the robot:")
        print("A: Go Forward, L: Turn Left, R: Turn Right, B: Go Back, E: Stop, O: Obstacle Avoidance, T: Line Follow")
        user_input = input("Your choice: ").strip().upper()
        # print(f"User command: {user_input}")

        if user_input == 'QW':
            print("Exiting program...")
            break
        else:
            send_command(user_input)
        # else:
        #     print("Invalid command. Please try again.")
