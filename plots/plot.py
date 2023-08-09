import matplotlib.pyplot as plt
import seaborn as sns
import pandas as pd

import plotting

#  UnionStrategy CompressStrategy Seed ElemsN UnionN TPL TPU RootChildrenCount DisjointSetCnt Elapsed
COLUMNS = ["union_strategy", "compress_strategy", "seed", "n", "N", "TPL", "TPU", "root_children_cnt", "set_cnt", "elapsed"]
df1 = pd.read_csv(
        'single_round__all_compstrats__n_upto_13.csv',
        header=None,
        names=COLUMNS
        )

df2 = pd.read_csv(
        'single_round__all_compstrats_except_NC__n_14.csv',
        header=None,
        names=COLUMNS
        )

measurements_df = pd.concat([df1, df2], ignore_index=True)
del df1
del df2

n10_df = measurements_df[measurements_df['n'] == (1 << 10)]
n12_df = measurements_df[measurements_df['n'] == (1 << 12)]
n13_df = measurements_df[measurements_df['n'] == (1 << 13)]
n14_df = measurements_df[measurements_df['n'] == (1 << 14)]
del measurements_df

n10_noNC_df = n10_df[n10_df['compress_strategy'] != 'NC']
n12_noNC_df = n12_df[n12_df['compress_strategy'] != 'NC']
n13_noNC_df = n13_df[n13_df['compress_strategy'] != 'NC']
n14_noNC_df = n14_df[n14_df['compress_strategy'] != 'NC']

df1 = pd.read_csv(
        'multi_round__all_compstrats_except_NC__n_10_12incomplete.csv',
        header=None,
        names=COLUMNS
        )

exectime_n10_noNC_df = df1[df1['n'] == (1 << 10)]
del df1

colors = sns.color_palette()
colors2 = sns.color_palette('pastel')

palette = {'NC': colors[0], 'FC': colors[1], 'PS': colors[2], 'PH': colors[3], 'R2': colors[4]}
palette2 = {'NC': colors[0], 'FC': colors2[1]}

if True:
    for n, df in ((10,n10_noNC_df), (12,n12_noNC_df), (13,n13_noNC_df), (14,n14_noNC_df)):
        with plotting.show(save=f"n{n}_noNC__TPL_over_N__hue_C__col_U__loglog_scale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['TPL']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='TPL promedio')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)
            plt.yscale('log', base=2)

if True:
    for n, df in ((10,n10_noNC_df), (12,n12_noNC_df), (13,n13_noNC_df), (14,n14_noNC_df)):
        with plotting.show(save=f"n{n}_noNC__TPL_over_N__hue_C__col_U__loglog_scale__zoomed.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['TPL']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='TPL promedio')
            plt.xscale('log', base=2)
            plt.yscale('log', base=2)
            plt.xlim(1 << (n-3), 1 << (n+3))
            plt.ylim(0.25)
            sns.move_legend(g, 'upper right', bbox_to_anchor=(1, 0.94))

if True:
    for n, df in ((10,n10_df), (12,n12_df), (13,n13_df)):
        df = df[df['compress_strategy'].isin(['NC', 'FC'])]
        with plotting.show(save=f"n{n}_onlyNC__TPL_over_N__hue_FC__col_U__loglog_scale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['TPL']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', hue_order=['FC', 'NC'], palette=palette2)
            g.set(ylabel='TPL promedio')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)
            plt.yscale('log', base=2)
            sns.move_legend(g, 'upper right', bbox_to_anchor=(1, 0.94))

if True:
    for n, df in ((10,n10_noNC_df), (12,n12_noNC_df), (13,n13_noNC_df), (14,n14_noNC_df)):
        with plotting.show(save=f"n{n}_noNC__TPU_over_N__hue_C__col_U__loglog_scale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['TPU']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='TPU promedio')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)
            plt.yscale('log', base=2)

if True:
    for n, df in ((10,n10_noNC_df), (12,n12_noNC_df), (13,n13_noNC_df), (14,n14_noNC_df)):
        with plotting.show(save=f"n{n}_noNC__TPU_over_N__hue_C__col_U__loglog_scale__zoomed.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['TPU']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='TPU promedio')
            plt.xscale('log', base=2)
            plt.yscale('log', base=2)
            plt.xlim(1 << (n-3), 1 << (n+3))
            plt.ylim(2**-6)


if True:
    for n, df in ((10,exectime_n10_noNC_df), ):
        with plotting.show(save=f"n{n}_noNC__exectime_over_N__hue_C__col_U__log_xscale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['elapsed']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='Tiempo de ejecucion promedio (seg)')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)

if True:
    for n, df in ((10,exectime_n10_noNC_df), ):
        with plotting.show(save=f"n{n}_noNC__exectime_over_N__hue_C__col_U__log_xscale__zoomed.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['elapsed']/df['n'], hue=df['compress_strategy'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='Tiempo de ejecucion promedio (seg)')
            plt.xscale('log', base=2)
            plt.xlim(1 << (n+7))
            plt.ylim(2e-7)

if True:
    for n, df in ((10,n10_noNC_df), (12,n12_noNC_df), (13,n13_noNC_df), (14,n14_noNC_df)):
        df1 = df.loc[:, ['compress_strategy', 'union_strategy', 'root_children_cnt', 'N']].rename(columns={'root_children_cnt': 'cnt'})
        df1['cnt_type'] = "Roots' children"

        df2 = df.loc[:, ['compress_strategy', 'union_strategy', 'set_cnt', 'N']].rename(columns={'set_cnt': 'cnt'})
        df2['cnt_type'] = 'Disjoint sets'
        df2.drop(df2[df2['compress_strategy'] != 'FC'].index, inplace=True)

        df = pd.concat([df1, df2], ignore_index=True)
        with plotting.show(save=f"n{n}_noNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['cnt'], hue=df['compress_strategy'], style=df['cnt_type'], err_style='bars', kind='line', col='union_strategy', palette=palette)
            g.set(ylabel='Cantidad')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)


if True:
    for n, df in ((10,n10_df), (12,n12_df), (13,n13_df),):
        df = df[df['compress_strategy'].isin(['NC', 'FC'])]

        df1 = df.loc[:, ['compress_strategy', 'union_strategy', 'root_children_cnt', 'N']].rename(columns={'root_children_cnt': 'cnt'})
        df1['cnt_type'] = "Roots' children"

        df2 = df.loc[:, ['compress_strategy', 'union_strategy', 'set_cnt', 'N']].rename(columns={'set_cnt': 'cnt'})
        df2['cnt_type'] = 'Disjoint sets'
        df2.drop(df2[df2['compress_strategy'] != 'FC'].index, inplace=True)

        df = pd.concat([df1, df2], ignore_index=True)
        with plotting.show(save=f"n{n}_onlyNC__Set_and_RChild_over_N__hue_C__style__Set_or_RChild__col_U__log_xscale.svg"):
            g = sns.relplot(data=df, x=df['N'], y=df['cnt'], hue=df['compress_strategy'], style=df['cnt_type'], err_style='bars', kind='line', col='union_strategy', hue_order=['FC', 'NC'], palette=palette2)
            g.set(ylabel='Cantidad')
            for ax in g.axes.flatten():
                ax.axvline((1 << n) * n, linestyle=":", color='black')
            plt.xscale('log', base=2)
            sns.move_legend(g, 'upper right', bbox_to_anchor=(1, 0.85))
