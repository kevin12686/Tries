import random

RAW_FILE = 'raw.txt'
Update_Ratio = 0.3

if __name__ == '__main__':
    v4_set = set()
    v6_set = set()
    with open(RAW_FILE, 'r', encoding='utf-8') as f:
        for line in f:
            subnet = line.split('|')[5]
            if ':' in subnet:
                v6_set.add('{}\n'.format(subnet))
            else:
                v4_set.add('{}\n'.format(subnet))

    v4_list = list(v4_set)
    v6_list = list(v6_set)

    random.shuffle(v4_list)
    random.shuffle(v6_list)

    v4_l = len(v4_list)
    v6_l = len(v6_list)

    v4_update = v4_list[:int(v4_l * Update_Ratio)]
    v6_update = v6_list[:int(v6_l * Update_Ratio)]

    v4_build = v4_list[int(v4_l * Update_Ratio):]
    v6_build = v6_list[int(v6_l * Update_Ratio):]

    with open('ipv4_build.txt', 'w', encoding='utf-8') as f:
        f.writelines(v4_build)

    with open('ipv6_build.txt', 'w', encoding='utf-8') as f:
        f.writelines(v6_build)

    with open('ipv4_update.txt', 'w', encoding='utf-8') as f:
        f.writelines(v4_update)

    with open('ipv6_update.txt', 'w', encoding='utf-8') as f:
        f.writelines(v6_update)
