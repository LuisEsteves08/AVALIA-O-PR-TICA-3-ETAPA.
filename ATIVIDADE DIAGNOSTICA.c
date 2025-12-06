#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

struct conta {
    int nconta;
    char titular[50];
    float saldo;
};

int main() {
    int opcao;
    struct conta contaAtual;
    int i;
    int encontrado;
    float valorOperacao;
    char randa[10];
    int numeroBusca;
    int contaOrigem, contaDestino;
    float valorTransferencia;
    int origemEncontrada, destinoEncontrada;
    float saldoOrigem, saldoDestino;
    char nomeOrigem[50], nomeDestino[50];
    int senhaAdm = 2508;
    int senhaDigitada;

    FILE *arquivo;
    srand(time(NULL));

    do {
        printf("\n            MENU          \n");
        printf("|1 - Criar conta-----------|\n");
        printf("|2 - Extrato---------------|\n");
        printf("|3 - Deposito--------------|\n");
        printf("|4 - Saque-----------------|\n");
        printf("|5 - Transferencia---------|\n");
        printf("|6 - Listar todas as contas|\n");
        printf("|7 - Sair------------------|\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            printf("Informe o nome do titular: ");
            scanf("%s", contaAtual.titular);

            for (i = 0; i < 9; i++) {
                randa[i] = (rand() % 10) + '0';
            }
            randa[9] = '\0';

            contaAtual.nconta = atoi(randa);
            contaAtual.saldo = 0;

            arquivo = fopen("contas.dat", "ab");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo!\n");
            } else {
                fwrite(&contaAtual, sizeof(struct conta), 1, arquivo);
                fclose(arquivo);

                printf("\nConta criada com sucesso!\n");
                printf("Titular: %s\n", contaAtual.titular);
                printf("Numero da conta: %d\n", contaAtual.nconta);
                printf("Saldo atual: R$ %.2f\n", contaAtual.saldo);
            }
        }

        else if (opcao == 2) {
            encontrado = 0;
            printf("Informe o numero da conta: ");
            scanf("%d", &numeroBusca);

            arquivo = fopen("contas.dat", "rb");
            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo!\n");
            } else {
                while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                    if (contaAtual.nconta == numeroBusca) {
                        encontrado = 1;
                        printf("\nExtrato da conta %d\n", contaAtual.nconta);
                        printf("Titular: %s\n", contaAtual.titular);
                        printf("Saldo: R$ %.2f\n", contaAtual.saldo);
                        break;
                    }
                }
                if (!encontrado) {
                    printf("Conta nao encontrada.\n");
                }
                fclose(arquivo);
            }
        }

        else if (opcao == 3) {
            printf("Informe o numero da conta: ");
            scanf("%d", &numeroBusca);

            encontrado = 0;
            arquivo = fopen("contas.dat", "rb+");

            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo\n");
            } else {
                while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                    if (contaAtual.nconta == numeroBusca) {
                        encontrado = 1;

                        printf("Saldo atual: R$ %.2f\n", contaAtual.saldo);
                        printf("Informe o valor do deposito: ");
                        scanf("%f", &valorOperacao);

                        contaAtual.saldo += valorOperacao;

                        fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                        fwrite(&contaAtual, sizeof(struct conta), 1, arquivo);

                        printf("Deposito realizado! Novo saldo: R$ %.2f\n", contaAtual.saldo);
                        break;
                    }
                }

                if (!encontrado) {
                    printf("Conta nao encontrada!\n");
                }

                fclose(arquivo);
            }
        }

        else if (opcao == 4) {
            printf("Informe o numero da conta: ");
            scanf("%d", &numeroBusca);

            encontrado = 0;
            arquivo = fopen("contas.dat", "rb+");

            if (arquivo == NULL) {
                printf("Erro ao abrir o arquivo ou nenhuma conta existente.\n");
            } else {
                while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                    if (contaAtual.nconta == numeroBusca) {
                        encontrado = 1;

                        printf("Saldo atual: R$ %.2f\n", contaAtual.saldo);
                        printf("Informe o valor do saque: ");
                        scanf("%f", &valorOperacao);

                        if (valorOperacao > contaAtual.saldo) {
                            printf("Saldo insuficiente!\n");
                        } else {
                            contaAtual.saldo -= valorOperacao;
                            fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                            fwrite(&contaAtual, sizeof(struct conta), 1, arquivo);
                            printf("Saque realizado! Novo saldo: R$ %.2f\n", contaAtual.saldo);
                        }
                        break;
                    }
                }
                if (!encontrado) {
                    printf("Conta nao encontrada!\n");
                }
                fclose(arquivo);
            }
        }

        else if (opcao == 5) {
            arquivo = fopen("contas.dat", "rb+");
            if (arquivo == NULL) {
                printf("Nenhuma conta cadastrada!\n");
            } else {
                printf("Informe o numero da conta de origem: ");
                scanf("%d", &contaOrigem);

                printf("Informe o numero da conta de destino: ");
                scanf("%d", &contaDestino);

                printf("Informe o valor da transferencia: ");
                scanf("%f", &valorTransferencia);

                if (contaOrigem == contaDestino) {
                    printf("Nao e possivel transferir para a mesma conta!\n");
                } else {
                    origemEncontrada = 0;
                    destinoEncontrada = 0;

                    rewind(arquivo);
                    while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                        if (contaAtual.nconta == contaOrigem) {
                            if (valorTransferencia > contaAtual.saldo) {
                                printf("Saldo insuficiente!\n");
                                origemEncontrada = 0;
                                break;
                            }
                            origemEncontrada = 1;
                            saldoOrigem = contaAtual.saldo;
                            strcpy(nomeOrigem, contaAtual.titular);
                            break;
                        }
                    }

                    if (origemEncontrada) {
                        rewind(arquivo);
                        while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                            if (contaAtual.nconta == contaDestino) {
                                destinoEncontrada = 1;
                                saldoDestino = contaAtual.saldo;
                                strcpy(nomeDestino, contaAtual.titular);
                                break;
                            }
                        }
                    }

                    if (!origemEncontrada) {
                        printf("Conta de origem nao encontrada!\n");
                    } else if (!destinoEncontrada) {
                        printf("Conta de destino nao encontrada!\n");
                    }

                    if (origemEncontrada && destinoEncontrada) {
                        saldoOrigem -= valorTransferencia;
                        saldoDestino += valorTransferencia;

                        rewind(arquivo);
                        while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                            if (contaAtual.nconta == contaOrigem) {
                                contaAtual.saldo = saldoOrigem;
                                fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                                fwrite(&contaAtual, sizeof(struct conta), 1, arquivo);
                                break;
                            }
                        }

                        rewind(arquivo);
                        while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                            if (contaAtual.nconta == contaDestino) {
                                contaAtual.saldo = saldoDestino;
                                fseek(arquivo, -sizeof(struct conta), SEEK_CUR);
                                fwrite(&contaAtual, sizeof(struct conta), 1, arquivo);
                                break;
                            }
                        }

                        printf("Transferencia realizada!\n");
                    }
                }
                fclose(arquivo);
            }
        }

        else if (opcao == 6) {
            printf("Informe a senha de administrador: ");
            scanf("%d", &senhaDigitada);

            if (senhaDigitada == senhaAdm) {
                arquivo = fopen("contas.dat", "rb");
                if (arquivo == NULL) {
                    printf("Nenhuma conta cadastrada!\n");
                } else {
                    printf("\n--- LISTA DE CONTAS ---\n");
                    while (fread(&contaAtual, sizeof(struct conta), 1, arquivo)) {
                        printf("Conta: %d | Titular: %s | Saldo: R$ %.2f\n",
                               contaAtual.nconta, contaAtual.titular, contaAtual.saldo);
                    }
                    printf("-----------------------\n");
                    fclose(arquivo);
                }
            } else {
                printf("Senha incorreta!\n");
            }
        }

    } while (opcao != 7);

    printf("Programa encerrado...\n");
    return 0;
}

