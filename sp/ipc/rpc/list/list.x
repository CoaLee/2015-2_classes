struct list {
  string  id<>;
  list    *next;
};

program LIST {
  version LIST_V1 {
    void INSERT(string) = 1;
    int DELETE(string) = 2;
    list GET(void) = 3;
    void DROP(void) = 4;
  } = 1;
} = 0xdeadbeef;
