import subprocess
import numpy as np
import matplotlib.pyplot as plt

#To run create out.csv files where line is [Random(), Uniform(5,10), Exponential(1), Normal(0,1)]
def main():
    with open('out.csv', 'r') as fout:
        # out = subprocess.run(['./test'], stdout=fout, stderr=ferr)
        fout.seek(0)
        output = fout.readlines()
        random_arr = []
        uniform_arr = []
        exponential_arr = []
        normal_arr = []
        random_mean = 0
        random_mean_arr = []
        random_sum = 0
        uniform_mean = 0
        uniform_sum = 0
        uniform_mean_arr = []

        for i, line in enumerate(output):
            random, uniform, exponential, normal = line.split(', ')
            random = random.strip()
            random_sum += float(random)
            random_mean = random_sum / (i + 1)
            uniform = uniform.strip()
            uniform_sum += float(uniform)
            uniform_mean = uniform_sum/(i + 1)
            exponential = exponential.strip()
            normal = normal.strip()
            random_arr.append(float(random))
            random_mean_arr.append(random_mean)
            uniform_arr.append(float(uniform))
            uniform_mean_arr.append(uniform_mean)
            exponential_arr.append(float(exponential))
            normal_arr.append(float(normal))


        np_random = np.asarray(random_arr)
        np_random_mean = np.asarray(random_mean_arr)
        np_uniform = np.asarray(uniform_arr)
        np_uniform_mean = np.asarray(uniform_mean_arr)
        np_exponential = np.asarray(exponential_arr)
        np_normal = np.asarray(normal_arr)

        # ----------------- Uniform Real Dist. Random(0,1) ----------------- #
        plt.title('RandomGenerator::Random()')
        plt.plot(np_random, c='gray', label='Hodnoty Random(0,1)')
        plt.plot(np_random_mean, c='crimson', label='Průměr hodnot Random(0,1)')
        plt.legend(loc='best')
        plt.xlabel('Pocet namerenych hodnot')
        plt.ylabel('Vysledek Random(0,1)')
        plt.yticks([(i/10) for i in range(0, 11)])
        plt.ylim((-0.10, 1.20))
        plt.grid(True)
        plt.savefig('random.png')
        plt.show()

        # ---------------- Uniform Real Dist. Uniform(5,10) ---------------- #
        plt.title('RandomGenerator::Uniform(5,10)')
        plt.plot(np_uniform, c='gray', label='Hodnoty Uniform(5,10)')
        plt.plot(np_uniform_mean, c='crimson', label='Průměr hodnot Uniform(5,10)')
        plt.legend(loc='best')
        plt.xlabel('Pocet namerenych hodnot')
        plt.ylabel('Vysledek Uniform(5,10)')
        plt.yticks([5+(i/2) for i in range(0, 11)])
        plt.ylim((4.50, 11.00))
        plt.grid(True)
        plt.savefig('uniform-5-10.png')
        plt.show()

        # -------------------- Exponential Distribution -------------------- #
        plt.title('RandomGenerator::Exponential(1)')
        np_exponential = np.sort(np_exponential)
        exponential_hist, exp_bins = np.histogram(np_exponential, bins=[(i/10) for i in range(0, 100)], density=True)
        exp_bin_centers = (exp_bins[1:] + exp_bins[:-1]) * 0.5
        plt.plot(exp_bin_centers, exponential_hist, c='crimson', label='Distribuční funkce pro Exponential(1).\n Počet dílkú n=100')
        plt.ylim((0.00, 0.60))
        plt.xticks([x for x in range(0, 11)])
        plt.xlabel('Hodnota dílku n')
        plt.ylabel('Pravděpodobnost hodnoty dílku n')
        plt.legend(loc='best')
        plt.grid(True)
        plt.savefig('exponential-1.png')
        plt.show()

        # ----------------------- Normal Distribution ----------------------- #
        plt.title('RandomGenerator::Normal(0,1)')
        np_normal = np.sort(np_normal)
        normal_hist, bins = np.histogram(np_normal, bins=100, density=True)
        normal_bin_centers = (bins[1:] + bins[:-1]) * 0.5
        plt.plot(normal_bin_centers, normal_hist, c='crimson', label='Distribuční funkce pro Normal(0,1)\n Počet dílkú n=100')
        plt.ylim((0.00, 0.50))
        plt.legend(loc='best')
        plt.xlabel('Hodnota dílku n')
        plt.ylabel('Pravděpodobnost hodnoty dílku n')
        plt.grid(True)
        plt.savefig('normal-0-1.png')
        plt.show()

        return 0


if __name__ == '__main__':
    main()
