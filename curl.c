#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <curl/curl.h>

#define CURL(meth, curl, url, chunk, verbose) \
curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST,  meth); \
curl_easy_setopt(curl, CURLOPT_URL,            url); \
curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION,  write_data); \
curl_easy_setopt(curl, CURLOPT_WRITEDATA,      chunk); \
curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1); \
curl_easy_setopt(curl, CURLOPT_NOSIGNAL,       1); \
curl_easy_setopt(curl, CURLOPT_VERBOSE,        verbose); \
curl_easy_perform(curl); \
return chunk->data;

#define SET_DATA(curl, data) \
curl_easy_setopt(curl, CURLOPT_POSTFIELDS, data); \
curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, strlen(data));

typedef struct {
  char   *data;
  size_t size;
} chunk_t;

static size_t write_data(void *buf, size_t size, size_t nmemb, void *data)
{
  size_t realsize = size * nmemb;
  chunk_t *chunk = (chunk_t *)data;

  char *tmp = realloc(chunk->data, chunk->size+realsize+1);
  if (tmp == NULL) exit(EXIT_FAILURE);
  chunk->data = tmp;

  memcpy(&(chunk->data[chunk->size]), buf, realsize);
  chunk->size += realsize;
  chunk->data[chunk->size] = 0;

  return realsize;
}

chunk_t * arc_curl_init_chunk(void)
{
  chunk_t *chunk = malloc(sizeof(chunk_t));
  if (chunk == NULL) exit(EXIT_FAILURE);
  chunk->data = NULL;
  chunk->size = 0;

  return chunk;
}

void arc_curl_cleanup_chunk(chunk_t *chunk)
{
  free(chunk->data);
  free(chunk);
}

char* arc_curl_get(CURL *curl, const char* url, chunk_t *chunk, int verbose)
{
  CURL("GET", curl, url, chunk, verbose)
}

char* arc_curl_post(CURL *curl, const char* url, const char* data, chunk_t *chunk, int verbose)
{
  SET_DATA(curl, data);
  CURL("POST", curl, url, chunk, verbose)
}

char* arc_curl_put(CURL *curl, const char* url, const char* data, chunk_t *chunk, int verbose)
{
  SET_DATA(curl, data);
  CURL("PUT", curl, url, chunk, verbose)
}

char* arc_curl_del(CURL *curl, const char* url, chunk_t *chunk, int verbose)
{
  CURL("DELETE", curl, url, chunk, verbose)
}
