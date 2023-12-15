# -*- coding: utf-8 -*-
"""
Create graphs from benchmarking
"""
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import io

# %%
f = open(r"\\wsl.localhost\Ubuntu\home\goose\UofT\CSC494\CSC494-Proj\final.txt", "r")

# first row is info
experiment_row = pd.read_csv(io.StringIO(f.readline()), header=None)

num_of_problems = experiment_row.iloc[0][0]
num_of_solvers = experiment_row.iloc[0][1]
num_of_repititions = experiment_row.iloc[0][2]
# num_of_repititions = 1

all_benchmarks = None

has_reached_end = False
problem_index = 0
while not has_reached_end:
    for repitition_index in range(num_of_repititions):
        next_line = f.readline()
        print(next_line)
        
        if next_line == "":
            has_reached_end = True
            break
        
        equation_row = pd.read_csv(io.StringIO(next_line), header=None)
        num_of_iterations = int(equation_row.iloc[0][0])
        num_of_terms = equation_row.iloc[0][1]
        num_of_zeroes_in_derivatives = equation_row.iloc[0][2]
        best_solution_value = equation_row.iloc[0][3]
        worst_solution_value = equation_row.iloc[0][4]
        randomized_selection = equation_row.iloc[0][5]
        
        for solver_index in range(num_of_solvers):
        
            name_of_solver = f.readline()
            
            raw_benchmark_info = ""
            for _ in range(num_of_iterations + 1):
                raw_benchmark_info += f.readline()
                
            benchmark_info = pd.read_csv(io.StringIO(raw_benchmark_info))
            benchmark_info['Solver'] = name_of_solver
            benchmark_info['problem_index'] = problem_index
            benchmark_info['repetition_index'] = repitition_index
            benchmark_info['num_of_terms'] = num_of_terms
            benchmark_info['num_of_cp'] = num_of_zeroes_in_derivatives
            
            benchmark_info['basis_metric'] = randomized_selection
            benchmark_info['best_guess'] = best_solution_value
            
            all_benchmarks = pd.concat([all_benchmarks, benchmark_info])
            
    problem_index += 1
        
all_benchmarks = all_benchmarks.reset_index(drop=True)
    
f.close()

# %%

# all_benchmarks['best_guess_deviation'] = (all_benchmarks['basis_metric'].astype(float) - all_benchmarks['Best_Guess'].astype(float))
all_benchmarks['best_guess_deviation'] = (all_benchmarks['Best_Guess'].astype(float) - all_benchmarks['basis_metric'].astype(float))
all_benchmarks['Solver'] = all_benchmarks['Solver'].str.strip()

# %%
# 6 17 snippet
solver_to_colour_mapping = {
    "Nelder Mead Solution": "orange",
    "PSO Solution": "green",
    "CMA ES Solution": "blue"
    }

mini_snippet = all_benchmarks[(all_benchmarks['num_of_terms'] == 6) & (all_benchmarks['num_of_cp'] == 17)]
mini_snippet = mini_snippet.groupby(['Solver', 'Function_Calls'])['best_guess_deviation'].mean().reset_index()
mini_snippet = mini_snippet[mini_snippet['Solver'] != 'Nelder Mead Solution'] # Removed Nelder mead because of inconsistent steps

fig, ax = plt.subplots()

for key, grp in mini_snippet.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='Function_Calls', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title("Function Calls vs. Average Deviation From Dumb Guess For 6 Terms and 17 zeroes")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()

# %%
mini_snippet = all_benchmarks[(all_benchmarks['num_of_terms'] == 3) & (all_benchmarks['num_of_cp'] == 6)]
mini_snippet = mini_snippet.groupby(['Solver', 'Function_Calls'])['best_guess_deviation'].mean().reset_index()
mini_snippet = mini_snippet[mini_snippet['Solver'] != 'Nelder Mead Solution'] # Removed Nelder mead because of inconsistent steps

fig, ax = plt.subplots()

for key, grp in mini_snippet.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='Function_Calls', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title("Function Calls vs. Average Deviation From Dumb Guess For 3 Terms and 6 zeroes")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()

# %%
# Finished Table
increasing_input = all_benchmarks.drop_duplicates(keep='first', subset=['Best_Guess', 'Solver', 'problem_index', 'repetition_index'])
increasing_input = increasing_input.drop_duplicates(keep='last', subset=['Solver', 'problem_index', 'repetition_index'])
increasing_input = increasing_input[increasing_input['best_guess_deviation'] < 100000000000]

# %%
# Increasing inputs look
num_of_terms_look = increasing_input.groupby(['Solver', 'num_of_terms', 'num_of_cp'])[['best_guess_deviation', 'Function_Calls']].mean().reset_index()
num_of_terms_look = num_of_terms_look.groupby(['Solver', 'num_of_terms'])['best_guess_deviation'].mean().reset_index()

fig, ax = plt.subplots()

for key, grp in num_of_terms_look.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='num_of_terms', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title("# Terms vs. Average Deviation From Guess")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()

# %%
# Increasing inputs look
num_of_zeroes_look = increasing_input.groupby(['Solver', 'num_of_terms', 'num_of_cp'])[['best_guess_deviation', 'Function_Calls']].mean().reset_index()
num_of_zeroes_look = num_of_zeroes_look.groupby(['Solver', 'num_of_cp'])['best_guess_deviation'].mean().reset_index()

fig, ax = plt.subplots()

for key, grp in num_of_zeroes_look.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='num_of_cp', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title(" # Critical Points vs. Average Deviation")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()

# %%
# Increasing inputs look
mini_look = all_benchmarks.drop_duplicates(keep='first', subset=['Best_Guess', 'Solver', 'problem_index', 'repetition_index'])
mini_look = mini_look.drop_duplicates(keep='last', subset=['Solver', 'problem_index', 'repetition_index'])
mini_look = mini_look[mini_look['best_guess_deviation'] < 100000000000]
mini_look = mini_look[(mini_look['num_of_terms'] < 4) & (mini_look["num_of_cp"] <= 7)]

# %%
num_of_terms_mini = mini_look.groupby(['Solver', 'num_of_terms', 'num_of_cp'])[['best_guess_deviation', 'Function_Calls']].mean().reset_index()
num_of_terms_mini = num_of_terms_mini.groupby(['Solver', 'num_of_terms'])['best_guess_deviation'].mean().reset_index()

fig, ax = plt.subplots()

for key, grp in num_of_terms_mini.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='num_of_terms', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title("# Terms vs. Average Deviation From Guess (Terms < 4, CP < 8)")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()

# %%
num_of_zeroes_mini = mini_look.groupby(['Solver', 'num_of_terms', 'num_of_cp'])[['best_guess_deviation', 'Function_Calls']].mean().reset_index()
num_of_zeroes_mini = num_of_zeroes_mini.groupby(['Solver', 'num_of_cp'])['best_guess_deviation'].mean().reset_index()

fig, ax = plt.subplots()

for key, grp in num_of_zeroes_mini.groupby(['Solver']):
    ax = grp.plot(ax=ax, kind='line', x='num_of_cp', y='best_guess_deviation', c=solver_to_colour_mapping[key], label=key)

plt.legend(loc='best')
plt.title("# Critical Points vs. Average Deviation From Guess (Terms < 4, CP < 8)")
plt.ylabel("[Solver Best - Random Selection Best]")
plt.show()