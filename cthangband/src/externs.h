/*
 * externs.h
 */

/*
 * Automatically generated function definitions
 */

#ifndef INCLUDED_EXTERNS_H
#define INCLUDED_EXTERNS_H


/* angband.h */

/* birth.c */
#if defined(BIRTH_C) || defined(FILES_C)
extern int maxstat(int race, int temp, int stat);
#endif
#if defined(BIRTH_C) || defined(FILES_C) || defined(GENERATE_C)
extern void create_random_name(int race, char *name);
#endif
#if defined(BIRTH_C)
extern void initialise_quests();
extern int get_number_monster(int i);
extern int get_rnd_q_monster(int q_idx);
extern void player_birth_quests(void);
#endif
#if defined(BIRTH_C) || defined(DUNGEON_C)
extern void player_birth(void);
#endif

/* cave.c */
#if defined(CAVE_C) || defined(CMD5_C) || defined(CMD6_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C)
extern int distance(int y1, int x1, int y2, int x2);
#endif
#if defined(CAVE_C) || defined(MELEE2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(XTRA1_C)
extern bool los(int y1, int x1, int y2, int x2);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern bool player_can_see_bold(int y, int x);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C)
extern bool no_lite(void);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool cave_valid_bold(int y, int x);
#endif
#if defined(CAVE_C)
extern void map_info(int y, int x, byte *ap, char *cp, byte *tap, char *tcp);
#endif
#if defined(CAVE_C) || defined(CMD2_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void move_cursor_relative(int row, int col);
#endif
#if defined(CAVE_C) || defined(CMD2_C) || defined(SPELLS1_C)
extern void print_rel(char c, byte a, int y, int x);
#endif
#if defined(CAVE_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C)
extern void note_spot(int y, int x);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void lite_spot(int y, int x);
#endif
#if defined(CAVE_C) || defined(XTRA1_C)
extern void prt_map(void);
extern void display_map(int *cy, int *cx);
extern void display_wild_map(uint xmin);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C)
extern void do_cmd_view_map(void);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(XTRA1_C)
extern void forget_lite(void);
#endif
#if defined(CAVE_C) || defined(XTRA1_C)
extern void update_lite(void);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(XTRA1_C)
extern void forget_view(void);
#endif
#if defined(CAVE_C) || defined(XTRA1_C)
extern void update_view(void);
#endif
#if defined(CAVE_C)
extern void forget_flow(void);
#endif
#if defined(CAVE_C) || defined(XTRA1_C)
extern void update_flow(void);
#endif
#if defined(CAVE_C) || defined(CMD5_C) || defined(CMD6_C) || defined(WIZARD2_C)
extern void map_area(void);
#endif
#if defined(CAVE_C) || defined(CMD5_C) || defined(CMD6_C) || defined(GENERATE_C) || defined(WIZARD2_C)
extern void wiz_lite(void);
#endif
#if defined(CAVE_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS2_C)
extern void wiz_dark(void);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void cave_set_feat(int y, int x, int feat);
#endif
#if defined(CAVE_C) || defined(CMD2_C) || defined(MELEE2_C) || defined(SPELLS1_C)
extern void mmove2(int *y, int *x, int y1, int x1, int y2, int x2);
#endif
#if defined(CAVE_C) || defined(MELEE2_C) || defined(XTRA2_C)
extern bool projectable(int y1, int x1, int y2, int x2);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void scatter(int *yp, int *xp, int y, int x, int d, int m);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void health_track(int m_idx);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void monster_race_track(int r_idx);
#endif
#if defined(CAVE_C) || defined(CMD5_C)
extern void object_kind_track(int k_idx);
#endif
#if defined(CAVE_C) || defined(XTRA1_C)
extern object_type *cnv_idx_to_obj(s16b index);
#endif
#if defined(CAVE_C)
extern s16b cnv_obj_to_idx(object_type *o_ptr);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD3_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(XTRA2_C)
extern void object_track(object_type *o_ptr);
#endif
#if defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void disturb(int stop_stealth, int unused_flag);
#endif
#if defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(QUEST_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool is_quest(int level);
#endif

/* cmd1.c */
#if defined(CMD1_C) || defined(CMD2_C)
extern bool test_hit_fire(int chance, int ac, int vis);
#endif
#if defined(CMD1_C)
extern bool test_hit_norm(int chance, int ac, int vis);
#endif
#if defined(CMD1_C) || defined(CMD2_C)
extern s16b critical_shot(int weight, int plus, int dam);
#endif
#if defined(CMD1_C)
extern s16b critical_norm(int weight, int plus, int dam);
#endif
#if defined(CMD1_C) || defined(CMD2_C)
extern s16b tot_dam_aux(object_type *o_ptr, int tdam, monster_type *m_ptr);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void search(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(MONSTER1_C) || defined(STORE_C) || defined(TABLES_C) || defined(WIZARD1_C)
extern void carry(int pickup);
#endif
#if defined(CMD1_C)
extern void touch_zap_player(monster_type *m_ptr);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C)
extern void py_attack(int y, int x);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(SPELLS1_C) || defined(SPELLS2_C)
extern void move_to(s16b y, s16b x);
#endif
#if defined(CMD1_C) || defined(CMD2_C)
extern void move_player(int dir, int do_pickup);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(DUNGEON_C)
extern void run_step(int dir);
#endif
#if defined(CMD1_C)
extern byte get_cur_town(y, x);
#endif

/* cmd2.c */
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_go_up(void);
extern void do_cmd_go_down(void);
extern void do_cmd_search(void);
extern void do_cmd_toggle_sneak(void);
#endif
#if defined(CMD2_C)
extern int chest_check(int y, int x);
#endif
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_open(void);
extern void do_cmd_close(void);
extern void do_cmd_tunnel(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C)
extern  bool easy_open_door(int y, int x);
extern bool do_cmd_disarm_aux(int y, int x, int dir);
#endif
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_disarm(void);
extern void do_cmd_bash(void);
extern void do_cmd_alter(void);
extern void do_cmd_spike(void);
extern void do_cmd_walk(int pickup);
extern void do_cmd_run(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(DUNGEON_C)
extern void do_cmd_stay(int pickup);
#endif
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_rest(void);
extern void do_cmd_fire(void);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(SPELLS2_C)
extern bool item_tester_hook_destroy(object_type *o_ptr);
#endif
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_throw(void);
#endif
#if defined(CMD2_C)
extern byte display_list(void (*display)(byte, byte *, char *), void (*confirm)(byte, char *), byte num, cptr string1, cptr string2, cptr substring);
#endif
#if defined(CMD2_C) || defined(DUNGEON_C)
extern void do_cmd_racial_power();
#endif

/* cmd3.c */
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_inven(void);
extern void do_cmd_equip(void);
extern void do_cmd_wield(void);
extern void do_cmd_takeoff(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C)
extern void do_cmd_drop(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_destroy(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C)
extern void destroy_pack(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_observe(void);
extern void do_cmd_uninscribe(void);
extern void do_cmd_inscribe(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C)
extern void do_cmd_refill(int item);
extern void do_cmd_target(void);
extern void do_cmd_look(void);
extern void do_cmd_locate(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_query_symbol(void);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C)
extern void do_cmd_handle(void);
#endif

/* cmd4.c */
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(STORE_C) || defined(WIZARD2_C)
extern void do_cmd_redraw(void);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_change_name(void);
extern void do_cmd_message_one(void);
extern void do_cmd_messages(void);
#endif
#if defined(CMD4_C)
extern s16b toggle_frequency(s16b current);
#endif
#if defined(BIRTH_C) || defined(CMD4_C)
extern void do_cmd_options_aux(int page, cptr info);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_options(void);
extern void do_cmd_pref(void);
extern void do_cmd_macros(void);
extern void do_cmd_visuals(void);
extern void do_cmd_colors(void);
extern void do_cmd_note(void);
extern void do_cmd_version(void);
extern void do_cmd_feeling(bool FeelingOnly);
extern void do_cmd_load_screen(void);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(STORE_C)
extern void do_cmd_save_screen(void);
#endif
#if defined(CMD4_C)
extern void do_cmd_knowledge_artifacts(void);
#endif
#if defined(CMD4_C) || defined(CMD5_C) || defined(MONSTER1_C) || defined(QUEST_C) || defined(SPELLS2_C)
extern void full_name(char * Name, bool plural, bool article, bool strangearticle);
#endif
#if defined(CMD4_C) || defined(FILES_C)
extern void do_cmd_knowledge_chaos_features(void);
#endif
#if defined(CMD4_C) || defined(XTRA1_C)
extern bool shops_good(int town);
extern void shops_display(int town);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_knowledge(void);
#endif

/* cmd4.h */

/* cmd5.c */
#if defined(CMD5_C)
extern bool cantrip_okay(int fav);
#endif
#if defined(CMD5_C) || defined(OBJECT2_C)
extern void print_cantrips(byte *spells, int num, int y, int x);
#endif
#if defined(CMD5_C) || defined(STORE_C)
extern int get_cantrip(int *sn, int sval);
#endif
#if defined(CMD5_C)
extern bool favour_okay(int fav,  int sphere);
extern s16b cantrip_chance(int ctp);
extern void print_favours(byte *spells, int num, int y, int x, int sphere);
extern s16b favour_chance(int fav,int sphere);
#endif
#if defined(CMD5_C) || defined(STORE_C)
extern int get_spirit(int *sn, cptr prompt, bool call);
#endif
#if defined(CMD5_C)
extern bool spirit_okay(int spirit, bool call);
extern void print_spirits(int *valid_spirits,int num,int y, int x);
extern void rustproof(void);
#endif
#if defined(CMD3_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_browse(int item);
#endif
#if defined(CMD5_C) || defined(STORE_C)
extern void do_cmd_study(void);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern void do_poly_wounds(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void do_poly_self(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C)
extern  void fetch(int dir, int wgt, bool require_los);
#endif
#if defined(CMD5_C)
extern void wild_magic(int spell);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C)
extern void do_cmd_cast(void);
extern void do_cmd_cantrip(void);
#endif
#if defined(CMD5_C)
extern s32b favour_annoyance(favour_type *f_ptr);
extern void annoy_spirit(spirit_type *s_ptr,u32b amount);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C)
extern void do_cmd_invoke(void);
#endif
#if defined(CMD5_C) || defined(OBJECT2_C)
extern void mindcraft_info(char *p, int power);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C)
extern void do_cmd_mindcraft(void);
#endif

/* cmd6.c */
#if defined(CMD3_C) || defined(CMD6_C) || defined(DUNGEON_C)
extern void do_cmd_eat_food(int item);
extern void do_cmd_quaff_potion(int item);
#endif
#if defined(CMD6_C) || defined(XTRA2_C)
extern bool curse_armor(void);
extern bool curse_weapon(void);
#endif
#if defined(CMD3_C) || defined(CMD6_C) || defined(DUNGEON_C)
extern void do_cmd_read_scroll(int item);
extern void do_cmd_use_staff(int item);
extern void do_cmd_aim_wand(int item);
extern void do_cmd_zap_rod(int item);
extern void do_cmd_activate(int item);
#endif

/* combat.c */
#if defined(COMBAT_C)
extern bool make_attack(s16b skill,s16b arm_type,s16b at_type, s16b at_size, s16b* dam,s16b* crit_type,s16b* crit_level);
extern void crit_damage(s16b table,s16b severity,s16b *stun,s16b *bleed, s16b *bone,s16b *organ,s16b *xtra);
#endif

/* config.h */

/* defines.h */

/* dungeon.c */
#if defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C)
extern cptr find_feeling(object_type *o_ptr);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern void change_level(s16b new_level, byte come_from);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C)
extern bool psychometry(void);
#endif
#if defined(BIRTH_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void curse(object_type *o_ptr);
#endif
#if defined(DUNGEON_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C)
extern void play_game(bool new_game);
#endif

/* files.c */
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(MAIN_C) || defined(WIZARD1_C)
extern void safe_setuid_drop(void);
extern void safe_setuid_grab(void);
#endif
#if defined(FILES_C)
extern s16b tokenize(char *buf, s16b num, char **tokens);
#endif
#if defined(BIRTH_C) || defined(FILES_C) || defined(INIT2_C)
extern errr add_stats(s16b sex, s16b race, s16b template, s16b maximise, s16b st, s16b in, s16b wi, s16b dx, s16b co, s16b ch, cptr name);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(OBJECT1_C)
extern errr process_pref_file_aux(char *buf);
#endif
#if defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(OBJECT1_C)
extern errr process_pref_file(cptr name);
#endif
#if defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_C)
extern errr check_time(void);
#endif
#if defined(FILES_C) || defined(MAIN_C)
extern errr check_time_init(void);
#endif
#if defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_C)
extern errr check_load(void);
#endif
#if defined(FILES_C) || defined(MAIN_C)
extern errr check_load_init(void);
#endif
#if defined(FILES_C) || defined(OBJECT1_C)
extern void weapon_stats(object_type *o_ptr, byte slay, s16b *tohit, s16b *todam, s16b *weap_blow, s16b *mut_blow, s32b *damage);
#endif
#if defined(FILES_C) || defined(XTRA1_C)
extern void print_equippy();
#endif
#if defined(FILES_C)
extern byte skill_colour(int skill_index);
#endif
#if defined(FILES_C) || defined(OBJECT1_C)
extern int equip_mod(int i);
#endif
#if defined(FILES_C)
extern void display_player_name_stats(void);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(XTRA1_C)
extern void display_player(int mode);
#endif
#if defined(FILES_C)
extern void dump_final_messages(FILE * OutFile);
#endif
#if defined(CMD4_C) || defined(FILES_C)
extern errr file_character(cptr name, bool full);
#endif
#if defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(STORE_C)
extern void do_cmd_help(cptr name);
#endif
#if defined(CMD4_C) || defined(FILES_C)
extern errr show_file(cptr name, cptr what);
#endif
#if defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_C)
extern void process_player_name(void);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C)
extern bool get_name(void);
#endif
#if defined(DUNGEON_C) || defined(FILES_C)
extern void do_cmd_suicide(void);
#endif
#if defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C)
extern void do_cmd_save_game(bool is_autosave);
#endif
#if defined(FILES_C)
extern long total_points(void);
#endif
#if defined(FILES_C) || defined(MAIN_C)
extern void display_scores(int from, int to);
#endif
#if defined(FILES_C) || defined(STORE_C)
extern void template_score(int ptemplate);
extern void race_score(int race_num);
#endif
#if defined(DUNGEON_C) || defined(FILES_C)
extern void close_game(void);
#endif
#if defined(FILES_C) || defined(MAIN_GCU_C)
extern void exit_game_panic(void);
#endif
#if defined(CMD1_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C)
extern errr get_rnd_line(char * file_name, char * output);
#endif
#if defined(DEFINES_H) || defined(FILES_C)
extern void signals_ignore_tstp_1(void);
extern void signals_handle_tstp_1(void);
extern void signals_init_1(void);
extern void signals_ignore_tstp_2(void);
extern void signals_handle_tstp_2(void);
extern void signals_init_2(void);
#endif

/* files.h */

/* generate.c */
#if defined(BIRTH_C) || defined(GENERATE_C)
extern void generate_spirit_names();
#endif
#if defined(GENERATE_C)
extern void replace_friend(int m_idx);
extern void replace_all_friends(void);
#endif
#if defined(CMD1_C) || defined(GENERATE_C) || defined(SPELLS2_C)
extern void replace_secret_door(int y, int x);
#endif
#if defined(DUNGEON_C) || defined(GENERATE_C)
extern void generate_cave(void);
#endif

/* h-basic.h */

/* h-config.h */

/* h-define.h */

/* h-system.h */

/* h-type.h */

/* init1.c */
#if defined(INIT1_C)
extern bool find_string_x(char *buf, cptr string);
extern errr do_get_string(char *buf, char this, char *all, char *output, u32b *this_size, u32b max_size, u16b *offset);
extern void clear_escapes(char *buf);
#endif
#if defined(INIT1_C) || defined(INIT2_C)
extern errr init_r_event_txt(FILE *fp, char *buf);
extern errr init_v_info_txt(FILE *fp, char *buf);
extern errr init_f_info_txt(FILE *fp, char *buf);
extern errr init_k_info_txt(FILE *fp, char *buf);
extern errr init_u_info_txt(FILE *fp, char *buf);
extern errr init_a_info_txt(FILE *fp, char *buf);
extern errr init_e_info_txt(FILE *fp, char *buf);
extern errr init_r_info_txt(FILE *fp, char *buf);
#endif

/* init2.c */
#if defined(INIT2_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_XXX_C) || defined(MAIN_C)
extern void init_file_paths(char *path);
extern void init_angband(void);
#endif

/* load.c */
#if defined(LOAD_C) || defined(LOADSAVE_H) || defined(SAVE_C)
extern bool has_flag(u16b flag);
#endif
#if defined(LOAD_C) || defined(SAVE_C)
extern errr rd_savefile_new(void);
#endif

/* loadsave.h */

/* main-acn.c */
#if defined(MAIN_ACN_C)
extern errr Term_xtra_acn(int n, int v);
extern errr Term_wipe_acn(int x, int y, int n);
extern void Term_init_acn(term *t);
#endif
#if defined(DEFINES_H) || defined(MAIN_ACN_C)
extern FILE *my_fopen_acn(const char *filename, const char *mode);
extern errr my_fclose_acn(FILE *fff);
extern int fd_make_acn(cptr file, int mode);
extern errr fd_kill_acn(cptr file);
extern errr fd_move_acn(cptr file, cptr what);
extern int fd_open_acn(cptr path, int flags);
extern errr fd_close_acn(int d);
#endif
#if defined(INIT2_C) || defined(MAIN_ACN_C) || defined(SAVE_C)
extern int access(const char *path, int mode);
#endif
#if defined(DEFINES_H) || defined(MAIN_ACN_C)
extern errr fd_read_ros(int d, char *buf, huge nbytes);
extern errr fd_write_ros(int d, const char *buf, huge nbytes);
extern errr fd_seek_acn(int fd, huge offset);
extern errr fd_lock_acn(int fd, int what);
extern errr path_temp_ros(char *buf, int max);
#endif

/* main-dos.c */
#if defined(MAIN_DOS_C)
extern void convert_grx_bitmap(int width, int height, unsigned char *src, unsigned char *dest);
extern FONT *import_grx_font(char *fname);
#endif
#if defined(MAIN_DOS_C) || defined(MAIN_C)
extern errr init_dos(void);
#endif

/* main-gcu.c */
#if defined(MAIN_GCU_C) || defined(MAIN_C)
extern errr init_gcu(int argc, char **argv);
#endif

/* main-ibm.c */
#if defined(DEFINES_H) || defined(MAIN_IBM_C)
extern void enable_graphic_font_286(void *font);
#endif
#if defined(MAIN_IBM_C)
extern unsigned  __dpmi_allocate_dos_memory(int size, unsigned *selector);
extern void __dpmi_free_dos_memory(unsigned sel);
extern void __dpmi_int(int intno, __dpmi_regs *dblock);
#endif
#if defined(DEFINES_H) || defined(MAIN_IBM_C)
extern void enable_graphic_font_386(const char *font);
#endif
#if defined(MAIN_IBM_C) || defined(MAIN_C)
extern errr init_ibm(void);
#endif

/* main-mac.c */

/* main-vme.c */
#if defined(MAIN_IBM_C) || defined(MAIN_VME_C)
extern void ScreenUpdateLine(int line);
extern void ScreenClear(void);
#endif
#if defined(MAIN_VME_C) || defined(MAIN_C)
extern errr init_vme(void);
#endif
#if defined(MAIN_VME_C)
extern getkey(void);
extern getkeybuf(void);
extern void InitConsole(void);
extern void TerminateConsole(void);
extern void ResetScrBuf(void);
extern void AddScrBuf(char * ptr, int len);
extern void GetAddr(int y, int x, char *stream);
extern char InKey(void);
extern char InKeyBuf(void);
extern void ResetDISP(void);
extern int kbhit(void);
extern void ShowLine(int y, int x, int len);
extern void LoadProfile(void);
#endif
#if defined(DEFINES_H) || defined(MAIN_VME_C)
extern int open_vme(char *name, int flags, int mode);
extern void close_vme(int fd);
extern int read_vme(int fd, char *buff, int bytes);
extern int write_vme(int fd, char *buff, int bytes);
extern long lseek_vme(int fd, long pos, int set);
extern void unlink_vme(char *filename);
#endif

/* main-win.c */
#if defined(MAIN_WIN_C)
extern bool angband_keymap_find(cptr str, char *out);
extern LRESULT FAR PASCAL AngbandWndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern LRESULT FAR PASCAL AngbandListProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
extern int FAR PASCAL WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow);
#endif

/* main-x11.c */
#if defined(MAIN_X11_C) || defined(MAIN_C)
extern errr init_x11(int argc, char *argv[]);
#endif

/* main-xxx.c */
#if defined(MAIN_XXX_C)
extern errr init_xxx(void);
#endif

/* main.c */

/* melee1.c */
#if defined(MELEE1_C) || defined(MELEE2_C)
extern bool make_attack_normal(int m_idx);
#endif

/* melee2.c */
#if defined(MELEE2_C) || defined(XTRA2_C)
extern void curse_equipment(int chance, int heavy_chance);
#endif
#if defined(MELEE2_C)
extern bool make_attack_spell(int m_idx);
#endif
#if defined(DUNGEON_C) || defined(MELEE2_C)
extern void process_monsters(void);
#endif

/* monster1.c */
#if defined(MONSTER1_C) || defined(WIZARD1_C)
extern void describe_death_events(int r_idx, cptr he, void (*out)(cptr), bool omniscient);
#endif
#if defined(CMD3_C) || defined(MONSTER1_C) || defined(XTRA2_C)
extern void screen_roff(int r_idx);
#endif
#if defined(MONSTER1_C) || defined(XTRA1_C)
extern void display_roff(int r_idx);
#endif

/* monster2.c */
#if defined(GENERATE_C) || defined(MONSTER2_C)
extern s16b place_ghost(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void delete_monster_idx(int i,bool visibly);
#endif
#if defined(GENERATE_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS2_C)
extern void delete_monster(int y, int x);
#endif
#if defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SAVE_C)
extern void compact_monsters(int size);
#endif
#if defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C)
extern void remove_non_pets(void);
#endif
#if defined(MONSTER2_C)
extern void wipe_m_list(void);
#endif
#if defined(LOAD_C) || defined(MONSTER2_C)
extern s16b m_pop(void);
#endif
#if defined(GENERATE_C) || defined(MONSTER2_C)
extern errr get_mon_num_prep(void);
#endif
#if defined(GENERATE_C) || defined(MONSTER2_C) || defined(SPELLS1_C)
extern s16b get_mon_num(int level);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD4_C) || defined(GENERATE_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void monster_desc(char *desc, monster_type *m_ptr, int mode);
#endif
#if defined(MONSTER2_C) || defined(SPELLS2_C)
extern void lore_do_probe(int m_idx);
#endif
#if defined(MONSTER2_C) || defined(XTRA2_C)
extern void lore_treasure(int m_idx, int num_item, int num_gold);
#endif
#if defined(MONSTER2_C)
extern void sanity_blast (monster_type * m_ptr, bool necro);
#endif
#if defined(CMD1_C) || defined(DUNGEON_C) || defined(GENERATE_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C)
extern void update_mon(int m_idx, bool full);
#endif
#if defined(MONSTER2_C) || defined(XTRA1_C)
extern void update_monsters(bool full);
#endif
#if defined(MONSTER2_C) || defined(XTRA2_C)
extern bool place_monster_one(int y, int x, int r_idx, bool slp, bool charm, bool force);
#endif
#if defined(CMD1_C) || defined(GENERATE_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(WIZARD2_C)
extern bool place_monster_aux(int y, int x, int r_idx, bool slp, bool grp, bool charm, bool force);
#endif
#if defined(GENERATE_C) || defined(MONSTER2_C)
extern bool place_monster(int y, int x, bool slp, bool grp);
extern void put_quest_monster(int r_idx);
#endif
#if defined(MONSTER2_C) || defined(WIZARD2_C)
extern bool alloc_horde(int y, int x);
#endif
#if defined(DUNGEON_C) || defined(GENERATE_C) || defined(MONSTER2_C)
extern bool alloc_monster(int dis, int slp);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool summon_specific(int y1, int x1, int lev, int type);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(XTRA2_C)
extern  bool summon_specific_friendly(int y1, int x1, int lev, int type, bool Group_ok);
#endif
#if defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C)
extern bool multiply_monster(int m_idx, bool charm, bool clone);
#endif
#if defined(CMD2_C) || defined(MONSTER2_C) || defined(SPELLS1_C)
extern void message_pain(int m_idx, int dam);
#endif
#if defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C)
extern void update_smart_learn(int m_idx, int what);
#endif

/* object1.c */
#if defined(OBJECT1_C) || defined(SPELLS2_C)
extern void get_table_name(char * out_string);
#endif
#if defined(FILES_C) || defined(OBJECT1_C)
extern s16b lookup_unident(byte p_id, byte s_id);
#endif
#if defined(DUNGEON_C) || defined(OBJECT1_C)
extern void flavor_init(void);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(OBJECT1_C)
extern void reset_visuals(void);
#endif
#if defined(CMD1_C) || defined(CMD3_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(LOAD_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C)
extern void object_flags(object_type *o_ptr, u32b *f1, u32b *f2, u32b *f3);
#endif
#if defined(DUNGEON_C) || defined(OBJECT1_C)
extern void object_info_known(object_type *j_ptr, object_type *o_ptr);
#endif
#if defined(OBJECT1_C)
extern cptr descr_base(byte p_id);
#endif
#if defined(FILES_C) || defined(MELEE2_C) || defined(OBJECT1_C)
extern void object_flags_known(object_type *o_ptr, u32b *f1, u32b *f2, u32b *f3);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void object_desc(char *buf, object_type *o_ptr, int pref, int mode);
#endif
#if defined(CMD4_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C)
extern void object_desc_store(char *buf, object_type *o_ptr, int pref, int mode);
#endif
#if defined(OBJECT1_C) || defined(WIZARD1_C)
extern cptr item_activation(object_type *o_ptr);
#endif
#if defined(OBJECT1_C)
extern int launcher_type(object_type *o_ptr);
#endif
#if defined(CMD3_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA1_C)
extern bool identify_fully_aux(object_type *o_ptr, byte flags);
#endif
#if defined(FILES_C) || defined(OBJECT1_C)
extern void identify_fully_file(object_type *o_ptr, FILE *fff);
#endif
#if defined(CMD1_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MELEE1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C)
extern s16b index_to_label(int i);
#endif
#if defined(OBJECT1_C)
extern s16b label_to_inven(int c);
extern s16b label_to_equip(int c);
#endif
#if defined(CMD3_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(XTRA1_C)
extern s16b wield_slot(object_type *o_ptr);
#endif
#if defined(OBJECT1_C)
extern cptr mention_use(int i);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS2_C)
extern cptr describe_use(int i);
#endif
#if defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT1_C)
extern bool item_tester_okay(object_type *o_ptr);
#endif
#if defined(OBJECT1_C) || defined(XTRA1_C)
extern void display_inven(void);
extern void display_equip(void);
#endif
#if defined(CMD3_C) || defined(FILES_C) || defined(OBJECT1_C)
extern void show_inven(void);
extern void show_equip(void);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C)
extern bool get_item(int *cp, cptr pmt, bool equip, bool inven, bool floor);
#endif

/* object2.c */
#if defined(MELEE2_C) || defined(OBJECT2_C)
extern void excise_object_idx(int o_idx);
#endif
#if defined(CMD1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void delete_object_idx(int o_idx);
#endif
#if defined(DUNGEON_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS2_C)
extern void delete_object(int y, int x);
#endif
#if defined(DUNGEON_C) || defined(OBJECT2_C)
extern void do_cmd_rotate_stack(void);
#endif
#if defined(DUNGEON_C) || defined(OBJECT2_C) || defined(SAVE_C)
extern void compact_objects(int size);
#endif
#if defined(DUNGEON_C) || defined(GENERATE_C) || defined(OBJECT2_C)
extern void wipe_o_list(void);
#endif
#if defined(LOAD_C) || defined(MELEE1_C) || defined(OBJECT2_C)
extern s16b o_pop(void);
#endif
#if defined(OBJECT2_C)
extern errr get_obj_num_prep(void);
#endif
#if defined(OBJECT2_C) || defined(STORE_C)
extern s16b get_obj_num(int level);
#endif
#if defined(BIRTH_C) || defined(CMD1_C) || defined(CMD2_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C)
extern void object_known(object_type *o_ptr);
#endif
#if defined(BIRTH_C) || defined(CMD6_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C)
extern void object_aware(object_type *o_ptr);
#endif
#if defined(CMD6_C) || defined(OBJECT2_C)
extern void object_tried(object_type *o_ptr);
#endif
#if defined(OBJECT2_C) || defined(SPELLS2_C)
extern s32b flag_cost(object_type * o_ptr, int plusses);
extern s32b object_value_real(object_type *o_ptr);
#endif
#if defined(CMD3_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C)
extern s32b object_value(object_type *o_ptr);
#endif
#if defined(OBJECT2_C) || defined(STORE_C)
extern int object_similar_2(object_type *o_ptr, object_type *j_ptr);
extern bool object_similar(object_type *o_ptr, object_type *j_ptr);
extern bool object_absorb_2(object_type *o_ptr, object_type *j_ptr);
#endif
#if defined(OBJECT2_C)
extern void object_absorb(object_type *o_ptr, object_type *j_ptr);
#endif
#if defined(BIRTH_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD6_C) || defined(FILES_C) || defined(INIT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern s16b lookup_kind(int tval, int sval);
#endif
#if defined(BIRTH_C) || defined(CMD2_C) || defined(FILES_C) || defined(LOAD_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void object_wipe(object_type *o_ptr);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD6_C) || defined(FILES_C) || defined(LOAD_C) || defined(MELEE1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void object_copy(object_type *o_ptr, object_type *j_ptr);
#endif
#if defined(BIRTH_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD6_C) || defined(FILES_C) || defined(MONSTER1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void object_prep(object_type *o_ptr, int k_idx);
#endif
#if defined(OBJECT2_C) || defined(WIZARD2_C)
extern void random_artifact_resistance(object_type * o_ptr);
#endif
#if defined(OBJECT2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void apply_magic(object_type *o_ptr, int lev, bool okay, bool good, bool great);
#endif
#if defined(CMD2_C) || defined(OBJECT2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool make_object(object_type *j_ptr, bool good, bool great);
#endif
#if defined(CMD2_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C)
extern void place_object(int y, int x, bool good, bool great);
#endif
#if defined(CMD2_C) || defined(OBJECT2_C) || defined(XTRA2_C)
extern bool make_gold(object_type *j_ptr);
#endif
#if defined(CMD2_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C)
extern void place_gold(int y, int x);
#endif
#if defined(CMD2_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern s16b drop_near(object_type *j_ptr, int chance, int y, int x);
#endif
#if defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void acquirement(int y1, int x1, int num, bool great);
#endif
#if defined(CMD1_C) || defined(OBJECT2_C) || defined(SPELLS2_C)
extern void pick_trap(int y, int x);
#endif
#if defined(GENERATE_C) || defined(OBJECT2_C) || defined(SPELLS1_C)
extern void place_trap(int y, int x);
#endif
#if defined(CMD6_C) || defined(OBJECT2_C)
extern void inven_item_charges(int item);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C)
extern void inven_item_describe(int item);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C)
extern void inven_item_increase(int item, int num);
extern void inven_item_optimize(int item);
#endif
#if defined(CMD6_C) || defined(OBJECT2_C)
extern void floor_item_charges(int item);
#endif
#if defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT2_C) || defined(SPELLS2_C)
extern void floor_item_describe(int item);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT2_C) || defined(SPELLS2_C)
extern void floor_item_increase(int item, int num);
extern void floor_item_optimize(int item);
#endif
#if defined(CMD1_C) || defined(OBJECT2_C) || defined(STORE_C)
extern bool inven_carry_okay(object_type *o_ptr);
#endif
#if defined(BIRTH_C) || defined(CMD1_C) || defined(CMD6_C) || defined(OBJECT2_C) || defined(STORE_C)
extern s16b inven_carry(object_type *o_ptr, bool final);
#endif
#if defined(CMD3_C) || defined(OBJECT2_C)
extern s16b inven_takeoff(int item, int amt);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(OBJECT2_C)
extern void inven_drop(int item, int amt);
#endif
#if defined(OBJECT2_C) || defined(XTRA1_C)
extern void combine_pack(void);
extern void reorder_pack(void);
extern void display_spell_list(void);
#endif
#if defined(CMD5_C) || defined(OBJECT2_C)
extern s16b spell_chance(int spell,int school);
extern bool spell_okay(int spell, bool known, int school);
extern void get_favour_info(char *p, int spell, int sphere);
extern void get_cantrip_info(char *p, int spell);
#endif
#if defined(CMD5_C) || defined(OBJECT2_C) || defined(STORE_C)
extern void print_spells(byte *spells, int num, int y, int x, int school);
#endif
#if defined(OBJECT2_C) || defined(XTRA1_C)
extern void display_koff(int k_idx);
#endif

/* quest.c */
#if defined(GENERATE_C) || defined(LOAD_C) || defined(QUEST_C)
extern int get_quest_monster(void);
#endif
#if defined(QUEST_C)
extern int get_max_monster(void);
#endif
#if defined(GENERATE_C) || defined(MONSTER2_C) || defined(QUEST_C) || defined(XTRA2_C)
extern int get_quest_number(void);
#endif
#if defined(CMD4_C) || defined(QUEST_C)
extern void print_quest_message(void);
#endif
#if defined(DUNGEON_C) || defined(QUEST_C)
extern void quest_discovery(void);
#endif
#if defined(QUEST_C)
extern int next_quest_level(void);
#endif

/* readdib.c */
#if defined(MAIN_WIN_C) || defined(READDIB_C) || defined(READDIB_H)
extern BOOL ReadDIB(HWND hWnd, LPSTR lpFileName, DIBINIT *pInfo);
#endif

/* readdib.h */

/* resource.h */

/* save.c */
#if defined(FILES_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(SAVE_C)
extern bool save_player(bool as_4_1_0);
#endif
#if defined(DUNGEON_C) || defined(SAVE_C)
extern bool load_player(void);
#endif

/* spells1.c */
#if defined(CMD1_C) || defined(SPELLS1_C)
extern s16b poly_r_idx(int r_idx);
#endif
#if defined(CMD1_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C)
extern void teleport_away(int m_idx, int dis);
#endif
#if defined(SPELLS1_C)
extern void teleport_to_player(int m_idx);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(WIZARD2_C)
extern void teleport_player(int dis);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(WIZARD2_C)
extern void teleport_player_to(int ny, int nx);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(MELEE2_C) || defined(SPELLS1_C)
extern void teleport_player_level(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void take_hit(int damage, cptr hit_from);
#endif
#if defined(CMD1_C) || defined(MELEE1_C) || defined(SPELLS1_C)
extern void acid_dam(int dam, cptr kb_str);
#endif
#if defined(MELEE1_C) || defined(SPELLS1_C)
extern void elec_dam(int dam, cptr kb_str);
#endif
#if defined(CMD1_C) || defined(MELEE1_C) || defined(SPELLS1_C)
extern void fire_dam(int dam, cptr kb_str);
#endif
#if defined(MELEE1_C) || defined(SPELLS1_C)
extern void cold_dam(int dam, cptr kb_str);
#endif
#if defined(SPELLS1_C) || defined(SPELLS2_C)
extern bool inc_stat(int stat);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C)
extern bool dec_stat(int stat, int amount, int permanent);
#endif
#if defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern bool res_stat(int stat);
#endif
#if defined(CMD5_C) || defined(MELEE1_C) || defined(SPELLS1_C)
extern bool apply_disenchant(int mode);
#endif
#if defined(CMD5_C) || defined(SPELLS1_C)
extern void chaos_feature_shuffle(void);
#endif
#if defined(CMD1_C) || defined(CMD5_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool project(int who, int rad, int y, int x, int dam, int typ, int flg);
#endif
#if defined(CMD2_C) || defined(CMD6_C) || defined(SPELLS1_C)
extern bool potion_smash_effect(int who, int y, int x, int o_sval);
#endif

/* spells2.c */
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool hp_player(int num);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern void warding_glyph(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern void explosive_rune(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool do_dec_stat(int stat);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(XTRA2_C)
extern bool do_res_stat(int stat);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool do_inc_stat(int stat);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C)
extern void identify_pack(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool remove_curse(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern bool remove_all_curse(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool restore_level(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT1_C) || defined(SPELLS2_C)
extern bool alchemy(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern void self_knowledge(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS2_C)
extern bool lose_all_info(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern void mark_traps(void);
extern bool detect_traps(void);
extern bool detect_doors(void);
extern bool detect_stairs(void);
extern bool detect_treasure(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool detect_objects_gold(void);
extern bool detect_objects_normal(void);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool detect_objects_magic(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS2_C)
extern bool detect_monsters_normal(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool detect_monsters_invis(void);
extern bool detect_monsters_evil(void);
#endif
#if defined(SPELLS2_C)
extern bool detect_monsters_string(cptr Match);
extern bool detect_monsters_xxx(u32b match_flag);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern bool detect_all(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern void stair_creation(void);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool item_tester_hook_armour(object_type *o_ptr);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(STORE_C)
extern bool enchant(object_type *o_ptr, int n, int eflag);
extern bool enchant_spell(int num_hit, int num_dam, int num_ac);
#endif
#if defined(SPELLS2_C)
extern void curse_artifact (object_type * o_ptr);
extern void random_plus (object_type * o_ptr, bool is_scroll);
#endif
#if defined(OBJECT2_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void random_resistance (object_type * o_ptr, bool is_scroll, int specific);
#endif
#if defined(SPELLS2_C)
extern void random_misc (object_type * o_ptr, bool is_scroll);
extern void random_slay (object_type * o_ptr, bool is_scroll);
extern void give_activation_power (object_type * o_ptr);
extern void get_random_name(char * return_name, bool armour, int power);
#endif
#if defined(OBJECT2_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool create_artifact(object_type *o_ptr, bool a_scroll);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C)
extern bool artifact_scroll();
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool ident_spell(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(WIZARD2_C)
extern bool identify_fully(void);
#endif
#if defined(CMD2_C) || defined(SPELLS2_C)
extern bool item_tester_hook_recharge(object_type *o_ptr);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT1_C) || defined(SPELLS2_C)
extern bool recharge(int num);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C)
extern bool speed_monsters(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool slow_monsters(int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool sleep_monsters(int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool banish_evil(int dist);
#endif
#if defined(SPELLS2_C)
extern bool turn_undead(int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool dispel_undead(int dam);
extern bool dispel_evil(int dam);
extern bool dispel_good(int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool dispel_monsters(int dam);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool dispel_living(int dam);
extern bool dispel_demons(int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(MELEE2_C) || defined(SPELLS2_C)
extern void aggravate_monsters(int who);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool genocide(bool player_cast);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool mass_genocide(bool player_cast);
#endif
#if defined(CMD2_C) || defined(CMD6_C) || defined(OBJECT1_C) || defined(SPELLS2_C)
extern bool probing(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void destroy_area(int y1, int x1, int r, bool full);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(OBJECT1_C) || defined(SPELLS2_C)
extern void earthquake(int cy, int cx, int r);
#endif
#if defined(SPELLS2_C)
extern void lite_room(int y1, int x1);
#endif
#if defined(MELEE2_C) || defined(SPELLS2_C)
extern void unlite_room(int y1, int x1);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool lite_area(int dam, int rad);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(SPELLS2_C)
extern bool unlite_area(int dam, int rad);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern bool fire_ball(int typ, int dir, int dam, int rad);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool fire_bolt(int typ, int dir, int dam);
extern bool fire_beam(int typ, int dir, int dam);
extern bool fire_bolt_or_beam(int prob, int typ, int dir, int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool lite_line(int dir);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool drain_life(int dir, int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool wall_to_mud(int dir);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool wizard_lock(int dir);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool destroy_door(int dir);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C)
extern bool disarm_trap(int dir);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool heal_monster(int dir);
extern bool speed_monster(int dir,int dam);
extern bool slow_monster(int dir,int dam);
extern bool sleep_monster(int dir,int dam);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool stasis_monster(int dir,int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool confuse_monster(int dir, int plev);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool stun_monster(int dir, int plev);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool poly_monster(int dir,int dam);
extern bool clone_monster(int dir);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool fear_monster(int dir, int plev);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool death_ray(int dir, int plev);
#endif
#if defined(CMD6_C) || defined(SPELLS2_C)
extern bool teleport_monster(int dir);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool door_creation(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(MELEE2_C) || defined(SPELLS2_C)
extern bool trap_creation(void);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool glyph_creation(void);
extern bool wall_stone(void);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool destroy_doors_touch(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool sleep_monsters_touch(int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void call_chaos(int plev);
#endif
#if defined(CMD1_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void activate_ty_curse();
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void activate_hi_summon(void);
#endif
#if defined(CMD5_C) || defined(MELEE2_C) || defined(SPELLS2_C)
extern void summon_reaver(void);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern void wall_breaker(int plev);
extern void bless_weapon(void);
extern  bool detect_monsters_nonliving(void);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS2_C)
extern bool confuse_monsters(int dam);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool charm_monsters(int dam);
extern bool charm_animals(int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS2_C)
extern  bool stun_monsters(int dam);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern bool stasis_monsters(int dam);
extern  bool mindblast_monsters(int dam);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C) || defined(SPELLS2_C)
extern bool banish_monsters(int dist);
#endif
#if defined(CMD5_C) || defined(SPELLS2_C)
extern  bool turn_evil(int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern  bool turn_monsters(int dam);
#endif
#if defined(SPELLS2_C)
extern bool deathray_monsters(int dam);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool charm_monster(int dir, int plev);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C)
extern bool control_one_undead(int dir, int plev);
extern bool charm_animal(int dir, int plev);
#endif
#if defined(CMD2_C) || defined(SPELLS2_C)
extern void report_magics(void);
extern void teleport_swap(int dir);
#endif
#if defined(DUNGEON_C) || defined(SPELLS2_C)
extern void alter_reality(void);
#endif

/* store.c */
#if defined(STORE_C)
extern void room_rest(bool night);
extern bool free_homes(void);
#endif
#if defined(OBJECT2_C) || defined(STORE_C)
extern bool store_object_absorb(object_type *j_ptr, object_type *o_ptr);
#endif
#if defined(CMD4_C) || defined(STORE_C)
extern cptr store_title(int store_num);
#endif
#if defined(STORE_C)
extern s32b max_player_skill(void);
#endif
#if defined(DUNGEON_C) || defined(STORE_C)
extern void do_cmd_store(void);
extern void store_shuffle(int which);
#endif
#if defined(BIRTH_C) || defined(DUNGEON_C) || defined(STORE_C)
extern void store_maint(int which);
#endif
#if defined(BIRTH_C) || defined(STORE_C)
extern void store_init(int which);
#endif
#if defined(MELEE1_C) || defined(STORE_C)
extern void move_to_black_market(object_type * o_ptr);
#endif
#if defined(STORE_C)
extern int get_which_store(void);
extern void do_store_browse( object_type *o_ptr);
#endif

/* tables.c */

/* types.h */

/* util.c */
#if defined(DEFINES_H) || defined(UTIL_C)
extern char *memset_ang(char *s, int c, huge n);
#endif
#if defined(H_CONFIG_H) || defined(UTIL_C)
extern int stricmp(cptr a, cptr b);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern int usleep_ang(huge usecs);
#endif
#if defined(MAIN_C) || defined(UTIL_C)
extern void user_name(char *buf, int id);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern errr path_parse_1(char *buf, int UNUSED max, cptr file);
extern errr path_parse_2(char *buf, int max, cptr file);
extern errr path_temp_ang(char *buf, int max);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_DOS_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MONSTER1_C) || defined(UTIL_C) || defined(WIZARD1_C)
extern errr path_build(char *buf, int max, cptr path, cptr file);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern FILE *my_fopen_ang(cptr file, cptr mode);
extern errr my_fclose_ang(FILE *fff);
#endif
#if defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MAIN_MAC_C) || defined(UTIL_C)
extern errr my_fgets(FILE *fff, char *buf, huge n);
#endif
#if defined(FILES_C) || defined(UTIL_C)
extern errr my_fputs(FILE *fff, cptr buf, huge n);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern errr fd_kill_ang(cptr file);
extern errr fd_move_ang(cptr file, cptr what);
#endif
#if defined(UTIL_C)
extern errr fd_copy(cptr file, cptr what);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern int fd_make_ang(cptr file, int mode);
extern int fd_open_ang(cptr file, int flags);
extern errr fd_lock_ang(int fd, int what);
extern errr fd_seek_ang(int fd, huge n);
#endif
#if defined(UTIL_C)
extern errr fd_chop(int fd, huge n);
#endif
#if defined(DEFINES_H) || defined(UTIL_C)
extern errr fd_read_ang(int fd, char *buf, huge n);
extern errr fd_write_ang(int fd, cptr buf, huge n);
extern errr fd_close_ang(int fd);
#endif
#if defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C)
extern void move_cursor(int row, int col);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(MAIN_WIN_C) || defined(UTIL_C)
extern void text_to_ascii(char *buf, cptr str);
#endif
#if defined(CMD4_C) || defined(UTIL_C)
extern void ascii_to_text(char *buf, cptr str);
extern sint macro_find_exact(cptr pat);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C)
extern errr macro_add(cptr pat, cptr act);
#endif
#if defined(UTIL_C)
extern errr macro_init(void);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(MAIN_X11_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(WIZARD2_C)
extern void flush(void);
#endif
#if defined(BIRTH_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(STORE_C) || defined(TABLES_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void bell(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(XTRA2_C)
extern void sound(int val);
#endif
#if defined(UTIL_C) || defined(XTRA1_C)
extern bool screen_is_icky(void);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_DOS_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern char inkey(void);
#endif
#if defined(CMD3_C) || defined(FILES_C) || defined(LOAD_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(XTRA2_C)
extern s16b quark_add(cptr str);
#endif
#if defined(BIRTH_C) || defined(DEFINES_H) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SAVE_C) || defined(UTIL_C)
extern cptr quark_str(s16b i);
#endif
#if defined(CMD4_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA1_C)
extern s16b message_num(void);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(SAVE_C) || defined(UTIL_C) || defined(XTRA1_C)
extern cptr message_str(s16b age);
#endif
#if defined(BIRTH_C) || defined(CMD1_C) || defined(CMD3_C) || defined(CMD4_C) || defined(LOAD_C) || defined(STORE_C) || defined(UTIL_C)
extern void message_add(cptr str);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(COMBAT_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MAIN_DOS_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER1_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(QUEST_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void msg_print(cptr msg);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(COMBAT_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(GENERATE_C) || defined(INIT1_C) || defined(INIT2_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(QUEST_C) || defined(SAVE_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void msg_format(cptr fmt, ...);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C)
extern void c_put_str(byte attr, cptr str, int row, int col);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD3_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C)
extern void put_str(cptr str, int row, int col);
#endif
#if defined(CMD4_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA1_C)
extern void c_prt(byte attr, cptr str, int row, int col);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(LOAD_C) || defined(MAIN_ACN_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void prt(cptr str, int row, int col);
#endif
#if defined(MONSTER1_C) || defined(UTIL_C)
extern void c_roff(byte a, cptr str);
extern void roff(cptr str);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA1_C)
extern void clear_from(int row);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C)
extern bool askfor_aux(char *buf, int len);
#endif
#if defined(BIRTH_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C)
extern bool get_string(cptr prompt, char *buf, int len);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C)
extern bool get_check(cptr prompt);
#endif
#if defined(CMD2_C) || defined(CMD3_C) || defined(CMD5_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool get_com(cptr prompt, char *command);
#endif
#if defined(CMD3_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(UTIL_C)
extern s16b get_quantity(cptr prompt, int max,bool allbydefault);
#endif
#if defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(UTIL_C)
extern void pause_line(int row);
#endif
#if defined(DUNGEON_C) || defined(STORE_C) || defined(UTIL_C)
extern void request_command(bool shopping);
#endif
#if defined(CMD4_C) || defined(MONSTER2_C) || defined(OBJECT1_C) || defined(UTIL_C) || defined(XTRA2_C)
extern bool is_a_vowel(int ch);
#endif
#if defined(CMD3_C) || defined(CMD4_C) || defined(UTIL_C) || defined(XTRA2_C)
extern int get_keymap_dir(char ch);
#endif
#if defined(CMD5_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(XTRA2_C)
extern  void repeat_push(int what);
extern  bool repeat_pull(int *what);
#endif
#if defined(DUNGEON_C) || defined(STORE_C) || defined(UTIL_C)
extern  void repeat_check(void);
#endif

/* variable.c */

/* variable.h */

/* wizard1.c */
#if defined(OBJECT2_C) || defined(WIZARD1_C)
extern bool make_fake_artifact(object_type *o_ptr, int name1);
#endif
#if defined(WIZARD1_C) || defined(WIZARD2_C)
extern void do_cmd_spoilers(void);
#endif

/* wizard2.c */
#if defined(CMD5_C) || defined(CMD6_C) || defined(WIZARD2_C)
extern void do_cmd_rerate(void);
#endif
#if defined(DUNGEON_C) || defined(WIZARD2_C)
extern void do_cmd_debug(void);
#endif
#if defined(WIZARD2_C)
extern void do_cmd_wiz_help(void);
#endif

/* xtra1.c */
#if defined(FILES_C) || defined(XTRA1_C)
extern void day_to_date(s16b day,char *date);
#endif
#if defined(BIRTH_C) || defined(FILES_C) || defined(XTRA1_C)
extern void cnv_stat(int val, char *out_val);
#endif
#if defined(BIRTH_C) || defined(OBJECT1_C) || defined(XTRA1_C)
extern s16b modify_stat_value(int value, int amount);
#endif
#if defined(XTRA1_C)
extern void prt_time(void);
extern void win_shops_display(void);
extern void win_visible_display(void);
#endif
#if defined(OBJECT1_C) || defined(XTRA1_C)
extern int mystic_armour(int slot);
extern bool cumber_glove(object_type *o_ptr);
extern bool cumber_helm(object_type *o_ptr);
extern int wield_skill(byte tval, byte sval);
#endif
#if defined(FILES_C) || defined(XTRA1_C)
extern byte ammunition_type(object_type *o_ptr);
#endif
#if defined(DUNGEON_C) || defined(STORE_C) || defined(XTRA1_C)
extern void notice_stuff(void);
#endif
#if defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void update_stuff(void);
#endif
#if defined(DUNGEON_C) || defined(SPELLS2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void redraw_stuff(void);
#endif
#if defined(DUNGEON_C) || defined(UTIL_C) || defined(XTRA1_C)
extern void window_stuff(void);
#endif
#if defined(DUNGEON_C) || defined(STORE_C) || defined(XTRA1_C)
extern void toggle_inven_equip(void);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(CMD6_C) || defined(FILES_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void handle_stuff(void);
#endif
#if defined(CMD1_C) || defined(OBJECT1_C) || defined(XTRA1_C)
extern bool ma_empty_hands();
#endif
#if defined(OBJECT1_C) || defined(XTRA1_C)
extern bool ma_heavy_armor();
#endif
#if defined(XTRA1_C)
extern void update_skill_maxima();
#endif
#if defined(FILES_C) || defined(XTRA1_C)
extern bool skill_check_possible(int index);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void skill_exp(int index);
#endif
#if defined(CMD5_C) || defined(XTRA1_C)
extern void gain_spell_exp(magic_type *spell);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(OBJECT2_C) || defined(XTRA1_C)
extern u16b spell_energy(u16b skill,u16b min);
#endif
#if defined(CMD5_C) || defined(OBJECT2_C) || defined(XTRA1_C)
extern byte spell_skill(magic_type *spell);
#endif

/* xtra2.c */
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_blind(int v);
#endif
#if defined(CMD1_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(STORE_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_confused(int v);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_poisoned(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_afraid(int v);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_paralyzed(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_image(int v);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern bool set_fast(int v);
#endif
#if defined(CMD1_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_slow(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_shield(int v);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_blessed(int v);
extern bool set_hero(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_shero(int v);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_protevil(int v);
extern bool set_shadow(int v);
extern bool set_invuln(int v);
extern bool set_tim_esp(int v);
extern bool set_tim_invis(int v);
#endif
#if defined(CMD6_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_tim_infra(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern bool set_oppose_acid(int v);
extern bool set_oppose_elec(int v);
extern bool set_oppose_fire(int v);
extern bool set_oppose_cold(int v);
extern bool set_oppose_pois(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_stun(int v);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_cut(int v);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool set_food(int v);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(XTRA2_C)
extern void gain_exp(s32b amount);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern void gain_skills(s32b amount);
#endif
#if defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C)
extern void lose_skills(s32b amount);
#endif
#if defined(MELEE2_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern void monster_death(int m_idx);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(MELEE1_C) || defined(SPELLS1_C) || defined(XTRA2_C)
extern bool mon_take_hit(int m_idx, int dam, bool *fear, cptr note);
#endif
#if defined(CMD3_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern void panel_bounds(void);
#endif
#if defined(DUNGEON_C) || defined(XTRA2_C)
extern void panel_bounds_center(void);
#endif
#if defined(CMD1_C) || defined(CMD3_C) || defined(DUNGEON_C) || defined(XTRA2_C)
extern void verify_panel(void);
#endif
#if defined(XTRA2_C)
extern cptr look_mon_desc(int m_idx);
extern void ang_sort_aux(vptr u, vptr v, int p, int q);
#endif
#if defined(CMD3_C) || defined(CMD4_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void ang_sort(vptr u, vptr v, int n);
#endif
#if defined(XTRA2_C)
extern bool target_able(int m_idx);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool target_okay(void);
#endif
#if defined(CMD3_C) || defined(XTRA2_C)
extern bool target_set(int mode);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(SPELLS2_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool get_aim_dir(int *dp);
#endif
#if defined(CMD2_C) || defined(XTRA2_C)
extern bool get_rep_dir(int *dp);
#endif
#if defined(XTRA2_C)
extern int get_chaos_patron();
#endif
#if defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C)
extern void gain_level_reward(int chosen_reward);
#endif
#if defined(CMD5_C) || defined(CMD6_C) || defined(XTRA2_C)
extern  bool tgt_pt(int *x,int *y);
#endif
#if defined(CMD5_C) || defined(DUNGEON_C) || defined(SPELLS1_C) || defined(WIZARD2_C) || defined(XTRA2_C)
extern bool gain_chaos_feature(int choose_mut);
#endif
#if defined(DUNGEON_C) || defined(XTRA2_C)
extern bool lose_chaos_feature(int choose_mut);
extern bool get_hack_dir(int *dp);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(XTRA2_C)
extern void dump_chaos_features(FILE * OutFile);
#endif

/* z-form.c */
#if defined(UTIL_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern uint vstrnfmt(char *buf, uint max, cptr fmt, va_list vp);
#endif
#if defined(Z_FORM_C) || defined(Z_FORM_H)
extern char *vformat(cptr fmt, va_list vp);
#endif
#if defined(CMD2_C) || defined(CMD5_C) || defined(STORE_C) || defined(UTIL_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern uint strnfmt(char *buf, uint max, cptr fmt, ...);
#endif
#if defined(MELEE2_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern uint strfmt(char *buf, cptr fmt, ...);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD1_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_DOS_C) || defined(MAIN_X11_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(STORE_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern char *format(cptr fmt, ...);
#endif
#if defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern void plog_fmt(cptr fmt, ...);
#endif
#if defined(FILES_C) || defined(INIT2_C) || defined(MAIN_DOS_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_FORM_C) || defined(Z_FORM_H)
extern void quit_fmt(cptr fmt, ...);
#endif
#if defined(Z_FORM_C) || defined(Z_FORM_H)
extern void core_fmt(cptr fmt, ...);
#endif

/* z-form.h */

/* z-rand.c */
#if defined(Z_RAND_C) || defined(Z_RAND_H)
extern  int Rand_bit(void);
extern  u32b Rand_u32b(void);
extern  u32b Rand_num(u32b m);
#endif
#if defined(DUNGEON_C) || defined(MAIN_DOS_C) || defined(Z_RAND_C) || defined(Z_RAND_H)
extern void Rand_state_init(u32b seed);
#endif
#if defined(Z_RAND_C) || defined(Z_RAND_H)
extern s32b Rand_mod(s32b m);
extern s32b Rand_div(s32b m);
#endif
#if defined(BIRTH_C) || defined(CMD5_C) || defined(GENERATE_C) || defined(OBJECT2_C) || defined(Z_RAND_C) || defined(Z_RAND_H)
extern s16b randnor(int mean, int stand);
#endif
#if defined(CMD1_C) || defined(CMD2_C) || defined(CMD5_C) || defined(CMD6_C) || defined(DUNGEON_C) || defined(MELEE1_C) || defined(MELEE2_C) || defined(MONSTER2_C) || defined(OBJECT2_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(XTRA2_C) || defined(Z_RAND_C) || defined(Z_RAND_H)
extern s16b damroll(int num, int sides);
#endif
#if defined(MONSTER2_C) || defined(Z_RAND_C) || defined(Z_RAND_H)
extern s16b maxroll(int num, int sides);
#endif

/* z-rand.h */

/* z-term.c */
#if defined(DUNGEON_C) || defined(STORE_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_user(int n);
#endif
#if defined(BIRTH_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(MAIN_GCU_C) || defined(MAIN_MAC_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_xtra(int n, int v);
#endif
#if defined(CAVE_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern void Term_queue_char(int x, int y, byte a, char c, byte ta, char tc);
#endif
#if defined(Z_TERM_C) || defined(Z_TERM_H)
extern void Term_queue_chars(int x, int y, int n, byte a, cptr s);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD2_C) || defined(CMD4_C) || defined(CMD5_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(LOAD_C) || defined(MAIN_ACN_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(SPELLS1_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(XTRA2_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_fresh(void);
#endif
#if defined(CAVE_C) || defined(DUNGEON_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_set_cursor(int v);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD3_C) || defined(CMD4_C) || defined(MONSTER1_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_gotoxy(int x, int y);
#endif
#if defined(CAVE_C) || defined(CMD4_C) || defined(OBJECT1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_draw(int x, int y, byte a, char c);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD3_C) || defined(MONSTER1_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_addch(byte a, char c);
#endif
#if defined(BIRTH_C) || defined(CMD3_C) || defined(CMD4_C) || defined(MONSTER1_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_addstr(int n, byte a, cptr s);
#endif
#if defined(CAVE_C) || defined(CMD4_C) || defined(FILES_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_putch(int x, int y, byte a, char c);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_putstr(int x, int y, int n, byte a, cptr s);
#endif
#if defined(BIRTH_C) || defined(CMD3_C) || defined(FILES_C) || defined(INIT2_C) || defined(MONSTER1_C) || defined(OBJECT1_C) || defined(OBJECT2_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_erase(int x, int y, int n);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_DOS_C) || defined(SAVE_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_clear(void);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_redraw(void);
#endif
#if defined(CAVE_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_get_cursor(int *v);
#endif
#if defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_get_size(int *w, int *h);
extern errr Term_locate(int *x, int *y);
#endif
#if defined(CMD4_C) || defined(FILES_C) || defined(UTIL_C) || defined(XTRA2_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_what(int x, int y, byte *a, char *c);
#endif
#if defined(MAIN_WIN_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_flush(void);
#endif
#if defined(MAIN_ACN_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_keypress(int k);
#endif
#if defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_key_push(int k);
#endif
#if defined(UTIL_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_inkey(char *ch, bool wait, bool take);
#endif
#if defined(BIRTH_C) || defined(CAVE_C) || defined(CMD2_C) || defined(CMD3_C) || defined(CMD4_C) || defined(CMD5_C) || defined(FILES_C) || defined(OBJECT1_C) || defined(SPELLS2_C) || defined(STORE_C) || defined(WIZARD1_C) || defined(WIZARD2_C) || defined(XTRA2_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_save(void);
extern errr Term_load(void);
#endif
#if defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_exchange(void);
#endif
#if defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_resize(int w, int h);
#endif
#if defined(CMD4_C) || defined(MAIN_ACN_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XXX_C) || defined(UTIL_C) || defined(XTRA1_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr Term_activate(term *t);
#endif
#if defined(MAIN_DOS_C) || defined(MAIN_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr term_nuke(term *t);
#endif
#if defined(MAIN_ACN_C) || defined(MAIN_DOS_C) || defined(MAIN_GCU_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_VME_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_XXX_C) || defined(Z_TERM_C) || defined(Z_TERM_H)
extern errr term_init(term *t, int w, int h, int k);
#endif

/* z-term.h */

/* z-util.c */
#if defined(XTRA1_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern void func_nothing(void);
#endif
#if defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern errr func_success(void);
extern errr func_problem(void);
extern errr func_failure(void);
#endif
#if defined(XTRA1_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern bool func_true(void);
#endif
#if defined(LOAD_C) || defined(SAVE_C) || defined(XTRA1_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern bool func_false(void);
#endif
#if defined(CAVE_C) || defined(CMD4_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT1_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_C) || defined(SPELLS2_C) || defined(UTIL_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern bool streq(cptr a, cptr b);
#endif
#if defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern bool suffix(cptr s, cptr t);
#endif
#if defined(FILES_C) || defined(MAIN_X11_C) || defined(SPELLS1_C) || defined(UTIL_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern bool prefix(cptr s, cptr t);
#endif
#if defined(INIT2_C) || defined(MAIN_ACN_C) || defined(MAIN_DOS_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(Z_FORM_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H) || defined(Z_VIRT_C)
extern void plog(cptr str);
#endif
#if defined(BIRTH_C) || defined(DUNGEON_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_ACN_C) || defined(MAIN_GCU_C) || defined(MAIN_IBM_C) || defined(MAIN_MAC_C) || defined(MAIN_WIN_C) || defined(MAIN_XXX_C) || defined(MAIN_C) || defined(TABLES_C) || defined(UTIL_C) || defined(Z_FORM_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H)
extern void quit(cptr str);
#endif
#if defined(MAIN_ACN_C) || defined(UTIL_C) || defined(Z_FORM_C) || defined(Z_UTIL_C) || defined(Z_UTIL_H) || defined(Z_VIRT_C)
extern void core(cptr str);
#endif

/* z-util.h */

/* z-virt.c */
#if defined(MAIN_WIN_C) || defined(Z_VIRT_C) || defined(Z_VIRT_H)
extern errr rnfree(vptr p, huge len);
#endif
#if defined(Z_VIRT_C) || defined(Z_VIRT_H)
extern vptr rpanic(huge len);
#endif
#if defined(MAIN_WIN_C) || defined(XTRA2_C) || defined(Z_VIRT_C) || defined(Z_VIRT_H)
extern vptr ralloc(huge len);
#endif
#if defined(BIRTH_C) || defined(CMD4_C) || defined(FILES_C) || defined(INIT2_C) || defined(MAIN_WIN_C) || defined(MAIN_X11_C) || defined(MAIN_C) || defined(OBJECT1_C) || defined(STORE_C) || defined(UTIL_C) || defined(Z_VIRT_C) || defined(Z_VIRT_H)
extern cptr string_make(cptr str);
extern errr string_free(cptr str);
#endif

/* z-virt.h */

/* unknown */

#endif
