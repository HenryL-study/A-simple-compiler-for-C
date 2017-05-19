const int len = 10;
int arr[10];

void popSort(){
    int i,j,flag,tmp;
    flag = 1;
    i = len - 2;
    while(i >= 0){
        if(flag){
            flag = 0;
            j = 0;
            while(j <= i){
                if(arr[j]>arr[j+1]){
                    tmp = arr[j];
                    arr[j] = arr[j+1];
                    arr[j+1] = tmp;
                    flag = 1;
                }
                j = j +1;
            }
        }
        i = i -1;
    }
}

int binary_search(int target){
    int low,high,mid;
    low = 0;high = len-1;
    popSort();
    while(low <= high){
        mid = (low+high)/2;
        if(arr[mid] > target)
            high = mid -1;
        if(arr[mid] < target)
            low = mid + 1;
        if(arr[mid] == target)
            return (mid+1);
    }
    return (-1);
}

void main(){
    int i,tmp,index;
    i = 0;
    printf("please input the array and the length is 10\n");
    while(i < len){
        scanf(tmp);
        arr[i] = tmp;
        i = i+1;
    }
    printf("please input the number you want to find\n");
    scanf(tmp);
    index = binary_search(tmp);
    if(index != -1){
        printf("after sort,the index of the number is",index);
        return;
    }
    printf("the input is not in the array\n");

}