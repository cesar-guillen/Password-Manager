#define AUTOGEN_PASS_LEN 20
#define SALT_LEN 16
#define HASH_LEN 64
#define HEX_SALT_LEN 16 * 2 + 1
#define HEX_HASH_LEN 64 * 2 + 1

void randomPasswordGeneration(int N, char *password);
double check_password_entropy(const char *password);
int from_hex(const char *in, unsigned char *out, size_t out_len);
void to_hex(const unsigned char *in, size_t len, char *out);
int generate_salt(unsigned char *salt);
int generate_hash(unsigned char *salt, unsigned char *hash, const char* master_pass);