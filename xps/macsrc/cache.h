struct cache
{
  struct cache *cache_next, *cache_prev;
  int fid;
  Matrix mat;				/* character -> device */
  int width, height, swidth, sheight; 	/* device coords */
};

struct char_table {
  Object 		char_key;
  struct cache 	*char_cache;
  
  struct device 	*char_device;
  UserPoint 		char_width;		    /* character coords */
};

