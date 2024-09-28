import subprocess
import pandas as pd
import random

elements = [50, 100, 200, 500, 1000]
p_values = [1/2, 1/2.71828182846, 1/4, 1/8]

df = pd.DataFrame(columns=['Element_count', 'P_Value', 'Average_Query_Distance'])

for element in elements:
    for p_value in p_values:
        total_query_distance = 0
        for i in range(10000):
            seed = random.randint(0, 100000000)
            command = f'./test-main {element} {seed} {p_value}'
            output = subprocess.check_output(command, shell=True).decode('utf-8')
            avg_query_distance = float(output.split('=')[-1].strip())
            total_query_distance += avg_query_distance

        avg_query_distance = total_query_distance / 10000
        new_row = pd.DataFrame({'Element_count': [element], 'P_Value': [p_value], 'Average_Query_Distance': [avg_query_distance]})
        df = pd.concat([df, new_row], ignore_index=True)

df.to_csv('result.csv', index=False)