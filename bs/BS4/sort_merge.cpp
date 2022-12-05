#include "sort_merge.h"
#include <utility>

using namespace std;

void quickSort(int *a, int li, int re) {
    if (li == re) { // Einelementig ist sortiert
        return;
    }
    size_t start = li, end = re;
    size_t m = li+(re-li)/2;
    int pivot = a[m];
    do {
        while (a[li] < pivot) {
            li += 1;
        }
        while (a[re] > pivot) {
            re -= 1;
        }
        if (li < re) {
            swap(a[li++], a[re--]); 
        } else if (li == re) {
            // Falls auf Pivot-Element, nicht am Rand splitten
            if (li == start) {
                li += 1;
            } else {
                re -= 1;
            }
        } 
    } while (li <= re);
    quickSort(a, start, re);
    quickSort(a, li, end);
}

void merge(int *seq, int *seq_sort, int len) {
  int l = 0, m = len / 2, i = 0;

  while (l < len / 2 && m < len)
    if (seq[l] < seq[m])
      seq_sort[i++] = seq[l++];
    else
      seq_sort[i++] = seq[m++];
  while (l < len / 2)
    seq_sort[i++] = seq[l++];
  while (m < len)
    seq_sort[i++] = seq[m++];
}
