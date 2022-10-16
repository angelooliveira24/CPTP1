#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define N 10000000
#define K 4

float *points, *clusters;
int iterations = 0;
int sizes[4];

void inicializa()
{
    // Each point and cluster are represented by 2 values x and y
    points   = (float*) malloc(N * 2 * sizeof(float));
    clusters = (float*) malloc(K * 2 * sizeof(float));

    srand(10);
    for(int i = 0; i < N * 2; i+=2)
    {
        points[i]     = (float) rand() / RAND_MAX;
        points[i + 1] = (float) rand() / RAND_MAX;
    }
    for(int i = 0; i < K * 2; i+=2)
    {
        clusters[i]     = points[i];
        clusters[i + 1] = points[i + 1];
    }
}

void kmeans()
{
    // Guardar o cluster a que cada ponto pertence
    int *attributions = (int*) malloc(N * sizeof(int));

    float aux;
    int flag = -1, cluster;

    while (flag != 1)
    {
        // Guardar a soma dos valores de cada coordenada dos pontos que pertencem a cada cluster
        float x[K] = {0.0, 0.0, 0.0, 0.0};
        float y[K] = {0.0, 0.0, 0.0, 0.0};

        // Guardar o número de pontos que pertencem a cada cluster
        int num_per_cluster[4] = {0, 0, 0, 0};

        if(flag == 0)
            flag = 1;

        // Percorrer todos os pontos
        for(int i = 0; i < N * 2; i+=2)
        {
            float min = 2.0;

            // Verificar para cada ponto o cluster onde este deve pertencer
            for(int j = 0; j < K * 2; j+=2)
            {
                // Distância eucladiana do ponto ao cluster
                aux = sqrtf(powf(points[i] - clusters[j], 2) + powf(points[i + 1] - clusters[j + 1], 2));

                // Se a distância for menor redefinir o mínimo e indicar o cluster com essa distância
                if(aux < min)
                {
                    min = aux;
                    cluster = j / 2;
                }
            }

            // Verfificar se o cluster do ponto alterou e se sim alterar a condição de paragem
            if(flag == 1 && attributions[i / 2] != cluster)
                flag = 0;

            // Alterar cluster associado ao ponto
            attributions[i / 2] = cluster;

            // Adicionar o valor das coordenadas do ponto para depois calcular a média
            x[cluster] += points[i];
            y[cluster] += points[i + 1];

            // Adionar o número total de pontos num determinado cluster
            num_per_cluster[cluster]++;
        }

        if(flag != 1)
        {
            for(int i = 0; i < K * 2; i+=2)
            {
                clusters[i]     = x[i / 2] / num_per_cluster[i / 2];
                clusters[i + 1] = y[i / 2] / num_per_cluster[i / 2];
            }
            flag = 0;
        }

        if(flag == 1)
        {
            sizes[0] = num_per_cluster[0];
            sizes[1] = num_per_cluster[1];
            sizes[2] = num_per_cluster[2];
            sizes[3] = num_per_cluster[3];
        }
        iterations++;
    }
    

    free(attributions);
}

int main(int argc, char const *argv[])
{
    inicializa();

    kmeans();

    printf("N = %d, K = %d\n", N, K);
    for(int i = 0; i < K * 2; i+=2)
        printf("Center: (%.3f, %.3f) : Size: %d\n", clusters[i], clusters[i + 1], sizes[i / 2]);
    printf("Iterations: %d\n", iterations);

    free(points);
    free(clusters);

    return 0;
}
