#define VARIABLE_H
/* File: externs.h */

/* Purpose: extern declarations (variables and functions) */

/*
 * Note that some files have their own header files
 * (z-virt.h, z-util.h, z-form.h, term.h, random.h)
 */


/*
 * Automatically generated "variable" declarations
 */
extern int full_grid;
/* tables.c */
extern s16b ddd[9];
extern s16b ddx[10];
extern s16b ddy[10];
extern s16b ddx_ddd[9];
extern s16b ddy_ddd[9];
extern char hexsym[16];
extern byte adj_val_min[];
extern byte adj_val_max[];
extern byte adj_mag_study[];
extern byte adj_mag_mana[];
extern byte adj_mag_fail[];
extern byte adj_mag_stat[];
extern byte adj_chr_gold[];
extern byte adj_int_dev[];
extern byte adj_wis_sav[];
extern byte adj_dex_dis[];
extern byte adj_int_dis[];
extern byte adj_dex_ta[];
extern byte adj_str_td[];
extern byte adj_dex_th[];
extern byte adj_str_th[];
extern byte adj_str_wgt[];
extern byte adj_str_hold[];
extern byte adj_str_dig[];
extern byte adj_str_blow[];
extern byte adj_dex_blow[];
extern byte adj_dex_safe[];
extern byte adj_con_fix[];
extern byte adj_con_mhp[];
extern byte blows_table[12][12];
extern owner_type owners[MAX_STORES_TOTAL][MAX_OWNERS];
extern u16b extract_energy[200];
extern player_sex sex_info[MAX_SEXES];
extern player_race race_info[MAX_RACES];
extern player_template template_info[MAX_TEMPLATE];
extern player_magic magic_info;
extern favour_type favour_info[MAX_SPHERE][32];
extern cantrip_type cantrip_info[32];
extern cptr favour_names[MAX_SPHERE][32];
extern cptr cantrip_names[32];
extern u32b spell_flags[4];
extern u32b cantrip_flags[7];
extern cptr spell_names[MAX_SCHOOL][32];
extern player_skill skill_set[MAX_SKILLS];
extern byte chest_traps[64];
extern cptr color_names[16];
extern cptr stat_names[6];
extern cptr stat_names_reduced[6];
extern cptr window_flag_desc[32];
extern option_type option_info[];
extern cptr chaos_patron_shorts[MAX_PATRON];
extern cptr chaos_patron_longs[MAX_PATRON];
extern int chaos_stats[MAX_PATRON];
extern int chaos_rewards[MAX_PATRON][20];
extern martial_arts ma_blows[MAX_MA];
extern mindcraft_power mindcraft_powers[MAX_MINDCRAFT_POWERS];
extern town_type town_defs[MAX_TOWNS];
extern dun_type dun_defs[MAX_CAVES];
extern wild_type wild_grid[12][12];
extern stat_default_type *stat_default;
extern s16b stat_default_total;

/* variable.c */
extern cptr copyright[5];
extern char syshelpfile[20];
extern byte version_major;
extern byte version_minor;
extern byte version_patch;
extern byte version_extra;
extern bool arg_fiddle;
extern bool arg_wizard;
extern bool arg_sound;
extern bool arg_graphics;
extern bool arg_force_original;
extern bool arg_force_roguelike;
extern bool character_generated;
extern bool character_dungeon;
extern bool character_loaded;
extern bool character_saved;
extern bool character_icky;
extern bool character_xtra;
extern u32b seed_flavor;
extern u32b seed_wild;
extern s16b command_cmd;
extern s16b command_arg;
extern s16b command_rep;
extern s16b command_dir;
extern s16b command_see;
extern s16b command_gap;
extern s16b command_wrk;
extern s16b command_new;
extern s16b energy_use;
extern s16b old_energy_use;
extern s16b choose_default;
extern bool create_up_stair;
extern bool create_down_stair;
extern bool msg_flag;
extern bool alive;
extern bool death;
extern s16b running;
extern s16b resting;
extern s16b cur_hgt;
extern s16b cur_wid;
extern s16b dun_level;
extern s16b dun_offset;
extern s16b dun_bias;
extern byte cur_town;
extern byte cur_dungeon;
extern byte recall_dungeon;
extern byte came_from;
extern s16b num_repro;
extern s16b object_level;
extern s16b monster_level;
extern s32b turn;
extern s32b old_turn;
extern s32b curse_turn;
extern bool cheat_wzrd;
extern bool use_sound;
extern bool use_graphics;
extern u16b total_winner;
extern u16b panic_save;
extern u16b noscore;
extern s16b signal_count;
extern bool inkey_base;
extern bool inkey_xtra;
extern bool inkey_scan;
extern bool inkey_flag;
extern s16b coin_type;
extern bool opening_chest;
extern bool shimmer_monsters;
extern bool shimmer_objects;
extern bool repair_monsters;
extern bool repair_objects;
extern s16b total_weight;
extern s16b inven_nxt;
extern s16b inven_cnt;
extern s16b equip_cnt;
extern s16b o_max;
extern s16b o_cnt;
extern s16b m_max;
extern s16b m_cnt;
extern s16b hack_m_idx;
extern s16b hack_m_idx_ii;
extern int total_friends;
extern s32b total_friend_levels;
extern byte current_function;
extern bool multi_rew;
extern char summon_kin_type;
extern bool hack_mind;
extern bool hack_chaos_feature;
extern int artifact_bias;
extern bool rogue_like_commands;
extern bool quick_messages;
extern bool quick_prompt;
extern bool other_query_flag;
extern bool carry_query_flag;
extern bool always_pickup;
extern bool always_repeat;
extern bool use_old_target;
extern bool depth_in_feet;
extern bool allow_quickstart;
extern bool allow_pickstats;
#if !defined(MACINTOSH) && !defined(WINDOWS) && !defined(ACORN)
extern bool display_credits;
#endif
extern bool use_color;
extern bool compress_savefile;
extern bool hilite_player;
extern bool ring_bell;
extern bool find_ignore_stairs;
extern bool find_ignore_doors;
extern bool find_cut;
extern bool stop_corner;
extern bool find_examine;
extern bool disturb_near;
extern bool disturb_move;
extern bool disturb_panel;
extern bool disturb_state;
extern bool disturb_dawn;
extern bool disturb_minor;
extern bool disturb_other;
extern bool avoid_abort;
extern bool avoid_other;
extern bool flush_disturb;
extern bool flush_failure;
extern bool flush_command;
extern bool fresh_before;
extern bool fresh_after;
extern bool fresh_message;
extern bool alert_hitpoint;
extern bool alert_failure;
extern bool view_yellow_lite;
extern bool view_bright_lite;
extern bool view_granite_lite;
extern bool view_special_lite;
extern bool skip_chaos_features;     /* Skip chaos features screen in 'C'haracter display */
extern bool plain_descriptions;
extern bool stupid_monsters;
extern bool auto_destroy;
extern bool wear_confirm;
extern bool confirm_wear_all;
extern bool confirm_stairs;
extern bool disturb_allies;
extern bool multi_stair;
extern bool maximise_mode;
extern bool preserve_mode;
extern bool use_autoroller;
extern bool spend_points;
extern bool ironman_shop;
extern bool unify_commands;
extern bool score_quitters;
extern bool centre_view;
extern bool no_centre_run;
extern bool view_perma_grids;
extern bool view_torch_grids;
extern bool flow_by_sound;
extern bool flow_by_smell;
extern bool track_follow;
extern bool track_target;
extern bool stack_allow_items;
extern bool stack_allow_wands;
extern bool stack_force_notes;
extern bool stack_force_notes_all;
extern bool stack_force_costs;
extern bool view_reduce_lite;
extern bool view_reduce_view;
extern bool auto_haggle;
extern bool auto_scum;
extern bool expand_look;
extern bool expand_list;
extern bool dungeon_align;
extern bool dungeon_stair;
extern bool dungeon_small;
extern bool smart_learn;
extern bool smart_cheat;
extern bool inscribe_depth;
extern bool show_labels;
extern bool show_weights;
extern bool show_choices;
extern bool show_details;
extern bool show_choices_main;
extern bool testing_stack;
extern bool testing_carry;
extern bool cheat_peek;
extern bool cheat_hear;
extern bool cheat_room;
extern bool cheat_xtra;
extern bool cheat_live;
extern bool cheat_skll;
extern bool last_words;              /* Zangband options */
extern bool speak_unique;
extern bool small_levels;
extern bool empty_levels;
extern bool player_symbols;
extern bool equippy_chars;
extern s16b hitpoint_warn;
extern s16b delay_factor;
extern s16b autosave_freq;
extern byte autosave_t;
extern byte autosave_l;
extern byte autosave_q;
extern s16b feeling;
extern s16b rating;
extern bool good_item_flag;
extern bool new_level_flag;
extern bool closing_flag;
extern s16b max_panel_rows, max_panel_cols;
extern s16b panel_row, panel_col;
extern s16b panel_row_min, panel_row_max;
extern s16b panel_col_min, panel_col_max;
extern s16b panel_col_prt, panel_row_prt;
extern s16b py;
extern s16b px;
extern s16b wildx;
extern s16b wildy;
extern s16b target_who;
extern s16b target_col;
extern s16b target_row;
extern s16b health_who;
extern s16b monster_race_idx;
extern s16b object_kind_idx;
extern s16b object_idx;
extern int player_uid;
extern int player_euid;
extern int player_egid;
extern char player_name[32];
extern char player_base[32];
extern char died_from[80];
extern char history[4][60];
extern char savefile[1024];
extern s16b lite_n;
extern byte lite_y[LITE_MAX];
extern byte lite_x[LITE_MAX];
extern s16b view_n;
extern byte view_y[VIEW_MAX];
extern byte view_x[VIEW_MAX];
extern s16b temp_n;
extern byte temp_y[TEMP_MAX];
extern byte temp_x[TEMP_MAX];
extern s16b macro__num;
extern cptr *macro__pat;
extern cptr *macro__act;
extern bool *macro__cmd;
extern char *macro__buf;
extern s16b quark__num;
extern cptr *quark__str;
extern u16b message__next;
extern u16b message__last;
extern u16b message__head;
extern u16b message__tail;
extern u16b *message__ptr;
extern char *message__buf;
extern moncol_type moncol[MAX_MONCOL];
extern u32b option_flag[8];
extern u32b option_mask[8];
extern window_type windows[8];
extern byte angband_color_table[256][4];
extern char angband_sound_name[SOUND_MAX][16];
extern cave_type *cave[MAX_HGT];
extern object_type *o_list;
extern monster_type *m_list;
extern monster_type *p_list;
extern quest q_list[MAX_QUESTS];  /* Heino Vander Sanden */
extern int MAX_Q_IDX;                              /* Heino Vander Sanden */
extern store_type *store;
extern object_type *inventory;
extern s16b alloc_kind_size;
extern alloc_entry *alloc_kind_table;
extern s16b alloc_race_size;
extern alloc_entry *alloc_race_table;
extern byte misc_to_attr[128];
extern char misc_to_char[128];
extern byte tval_to_attr[128];
extern char tval_to_char[128];
extern cptr keymap_act[KEYMAP_MODES][256];
extern player_type *p_ptr;
extern player_sex *sp_ptr;
extern player_race *rp_ptr;
extern player_template *cp_ptr;
extern player_magic *mp_ptr;
extern u32b spell_learned[MAX_SCHOOL];
extern u32b spell_worked[MAX_SCHOOL];
extern u32b spell_forgotten[MAX_SCHOOL];
extern byte spell_order[128];
extern s16b player_hp[100];
extern header *v_head;
extern vault_type *v_info;
extern char *v_name;
extern char *v_text;
extern header *f_head;
extern feature_type *f_info;
extern char *f_name;
extern char *f_text;
extern header *k_head;
extern object_kind *k_info;
extern char *k_name;
extern char *k_text;
extern header *u_head;
extern unident_type *u_info;
extern char *u_name;
extern s16b *pid_base;
extern header *a_head;
extern artifact_type *a_info;
extern char *a_name;
extern char *a_text;
extern header *e_head;
extern ego_item_type *e_info;
extern char *e_name;
extern char *e_text;
extern header *r_head;
extern monster_race *r_info;
extern char *r_name;
extern char *r_text;
extern header *event_head;
extern death_event_type *death_event;
extern char *event_name;
extern char *event_text;
extern cptr ANGBAND_SYS;
extern cptr ANGBAND_DIR;
extern cptr ANGBAND_DIR_APEX;
extern cptr ANGBAND_DIR_BONE;
extern cptr ANGBAND_DIR_DATA;
extern cptr ANGBAND_DIR_EDIT;
extern cptr ANGBAND_DIR_FILE;
extern cptr ANGBAND_DIR_HELP;
extern cptr ANGBAND_DIR_INFO;
extern cptr ANGBAND_DIR_SAVE;
extern cptr ANGBAND_DIR_USER;
extern cptr ANGBAND_DIR_XTRA;
extern bool item_tester_full;
extern byte item_tester_tval;
extern bool (*item_tester_hook)(object_type *o_ptr);
extern bool (*ang_sort_comp)(vptr u, vptr v, int a, int b);
extern void (*ang_sort_swap)(vptr u, vptr v, int a, int b);
extern bool (*get_mon_num_hook)(int r_idx);
extern bool (*get_obj_num_hook)(int k_idx);
extern bool angband_keymap_flag; /* Hack for main-win.c */
extern bool mystic_armour_aux;
extern bool mystic_notify_aux;
extern spirit_type spirits[MAX_SPIRITS];
extern tval_ammo_type tval_ammo[];
extern bool cheat_item;
extern bool spoil_art;
extern bool spoil_mon;
extern bool spoil_ego;
extern bool spoil_base;
extern bool spoil_value;
extern bool spoil_stat;
extern bool spoil_dam;
extern bool spoil_flag;
extern bool verbose_haggle;
extern bool bypass_prompt;




/*
 * Automatically generated "function declarations"
 */

/* birth.c */

/* cave.c */
#ifdef USE_TRANSPARENCY
#else /* USE_TRANSPARENCY */
#endif /* USE_TRANSPARENCY */

/* cmd1.c */

/* cmd2.c */

/* cmd3.c */

/* cmd4.c */
/* cmd5.c */

/* cmd6.c */

/* dungeon.c */

/* files.c */

/* generate.c */

/* init-txt.c */

/* init.c */
extern cptr explode_flags[];
extern cptr coin_types[];

/* load1.c */

/* load2.c */


/* melee1.c */
/* melee2.c */

/* monster1.c */

/* monster2.c */

/* object1.c */
/* object2.c */

/* save.c */

/* spells1.c */


/* spells2.c */


/* store.c */
/* util.c */

/* wizard1.c */

/* xtra1.c */

/* xtra2.c */
extern byte violet_uniques;

/* quest.c */


/*
 */

#ifdef SET_UID
/* util.c */
#endif

#ifndef HAS_MEMSET
/* util.c */
#endif

#ifndef HAS_STRICMP
/* util.c */
#endif

#ifndef HAS_USLEEP
/* util.c */
#endif

#ifdef MACINTOSH
/* main-mac.c */
#endif

#ifdef WINDOWS
/* main-win.c */
#endif
 
 #ifdef ALLOW_REPEAT /* TNB */
 
 /* util.c */
 
 #endif /* ALLOW_REPEAT -- TNB */
 
 #ifdef ALLOW_EASY_OPEN /* TNB */
 
 /* variable.c */
 extern bool easy_open;
 
 /* cmd2.c */
 
 #endif /* ALLOW_EASY_OPEN -- TNB */
 
 #ifdef ALLOW_EASY_DISARM /* TNB */
 
 /* variable.c */
 extern bool easy_disarm;
 
 /* cmd2.c */
 
 #endif /* ALLOW_EASY_DISARM -- TNB */
 
