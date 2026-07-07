local pos = get_position()

print("===\tlua script started\t===")

print(pos.x, pos.y, pos.z)

pos.y = pos.y + 10

set_position(pos.x + 1, pos.y, pos.z)

pos = get_position()

print(pos.x, pos.y, pos.z)

print("===\tlua script ended\t===")