#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Struct sesuai spesifikasi
typedef struct {
    int row;
    int col;
    int id;
    char rarity[15];
    int priority; // Priority kelangkaan: 3 = Legendary, 2 = Rare, 1 = Common
    int order;    // Urutan waktu penemuan oleh radar spiral
} Pokemon;

int grid[105][105];
int visited[105][105];
Pokemon list[10005];

int main() {
    int R, C;
    if (scanf("%d %d", &R, &C) != 2) return 0;

    for (int i = 0; i < R; i++) {
        for (int j = 0; j < C; j++) {
            scanf("%d", &grid[i][j]);
            visited[i][j] = 0;
        }
    }

    // Penentuan arah berdasarkan C (Ganjil vs Genap)
    // 0: Atas (-1, 0), 1: Kanan (0, 1), 2: Bawah (1, 0), 3: Kiri (0, -1)
    int dr[4], dc[4];
    if (C % 2 != 0) {
        // Ganjil: Atas -> Kanan -> Bawah -> Kiri
        dr[0] = -1; dc[0] = 0;
        dr[1] = 0;  dc[1] = 1;
        dr[2] = 1;  dc[2] = 0;
        dr[3] = 0;  dc[3] = -1;
    } else {
        // Genap: Kiri -> Atas -> Kanan -> Bawah
        dr[0] = 0;  dc[0] = -1;
        dr[1] = -1; dc[1] = 0;
        dr[2] = 0;  dc[2] = 1;
        dr[3] = 1;  dc[3] = 0;
    }

    // Titik pusat pemindaian
    int curr_r = R / 2;
    int curr_c = C / 2;

    int total_cells = R * C;
    int visited_count = 0;
    int pokemon_count = 0;
    int sweep_order = 0;

    int dir_idx = 0;
    int step_len = 1;
    int step_count = 0;

    // Pemindaian Spiral
    while (visited_count < total_cells) {
        // Proses sel jika berada dalam batas matriks dan belum dikunjungi
        if (curr_r >= 0 && curr_r < R && curr_c >= 0 && curr_c < C && !visited[curr_r][curr_c]) {
            visited[curr_r][curr_c] = 1;
            visited_count++;
            sweep_order++;

            int val = grid[curr_r][curr_c];

            // Pengecekan kategori Pokemon
            if (val > 0) {
                int is_common = (val % 6 == 0);
                int is_rare = (val % 7 == 0);

                if (is_common || is_rare) {
                    list[pokemon_count].row = curr_r;
                    list[pokemon_count].col = curr_c;
                    list[pokemon_count].id = val;
                    list[pokemon_count].order = sweep_order;

                    if (is_common && is_rare) {
                        strcpy(list[pokemon_count].rarity, "Legendary");
                        list[pokemon_count].priority = 3;
                    } else if (is_rare) {
                        strcpy(list[pokemon_count].rarity, "Rare");
                        list[pokemon_count].priority = 2;
                    } else {
                        strcpy(list[pokemon_count].rarity, "Common");
                        list[pokemon_count].priority = 1;
                    }
                    pokemon_count++;
                }
            }
        }

        // Pergerakan pola spiral
        curr_r += dr[dir_idx];
        curr_c += dc[dir_idx];
        step_count++;

        if (step_count == step_len) {
            step_count = 0;
            dir_idx = (dir_idx + 1) % 4; // Beralih ke arah berikutnya
            
            // Panjang langkah bertambah 1 setiap 2 kali perubahan arah
            if (dir_idx == 0 || dir_idx == 2) {
                step_len++;
            }
        }
    }

    // Jika tidak ada Pokemon terdeteksi
    if (pokemon_count == 0) {
        printf("Nggak ada Pokemon!\n");
        return 0;
    }

    // Pengurutan (Bubble Sort) berdasarkan Rarity (Legendary -> Rare -> Common)
    // Jika rarity sama, diurutkan berdasarkan waktu sapuan radar yang lebih awal (order)
    for (int i = 0; i < pokemon_count - 1; i++) {
        for (int j = 0; j < pokemon_count - i - 1; j++) {
            int swap_needed = 0;

            if (list[j].priority < list[j + 1].priority) {
                swap_needed = 1;
            } else if (list[j].priority == list[j + 1].priority) {
                if (list[j].order > list[j + 1].order) {
                    swap_needed = 1;
                }
            }

            if (swap_needed) {
                Pokemon temp = list[j];
                list[j] = list[j + 1];
                list[j + 1] = temp;
            }
        }
    }

    // Output Hasil
    for (int i = 0; i < pokemon_count; i++) {
        printf("[%s] ID: %d pada koordinat (%d, %d)\n",
               list[i].rarity, list[i].id, list[i].row, list[i].col);
    }

    return 0;
}