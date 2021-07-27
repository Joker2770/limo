/*************************************************************************
    > File Name: state_monitor.c
    > Author: Jintao Yang
    > Mail: 18608842770@163.com 
    > Created Time: 2021年07月16日 星期五 22时32分39秒
 ************************************************************************/

#include <gtk/gtk.h>
#include <string.h>

GtkWidget *window;
GtkWidget *label;
long long i = 0, tt = 0, tt0 = 0, idle = 0, idle0 = 0, r1 = 0, t1 = 0, r0 = 0, t0 = 0, mu = 0, mt = 0, mf = 0;

char *uptime(char *hms)
{
    int ss = 0, h = 0, m = 0, s = 0;
    float tt = 0.0f, it = 0.0f;
    FILE *fp = NULL;
    fp = fopen("/proc/uptime", "r");
    fscanf(fp, "%f %f", &tt, &it);
    fclose(fp);
    ss = (int)tt;
    h = ss / 3600;
    m = ss % 3600 / 60;
    s = ss % 3600 % 60;
    sprintf(hms, "%d:%02d:%02d", h, m, s);
    return hms;
}

void meminfo()
{
    FILE *fp = NULL;
    char cmt[40] = "", cmf[40] = "", a[40] = "", b[40] = "";
    fp = fopen("/proc/meminfo", "r");
    fgets(cmt, sizeof(cmt), fp);
    fgets(cmf, sizeof(cmf), fp);
    fclose(fp);
    sscanf(cmt, "%s%lld%s", a, &mt, b);
    sscanf(cmf, "%s%lld%s", a, &mf, b);
    //printf("MEM: %lldB/%lldB\n",mf*1024,mt*1024);
}

char *cpustat(char *cpusage)
{
    FILE *fp = NULL;
    char ch[100] = "", cpu[10] = "";
    fp = fopen("/proc/stat", "r");
    fgets(ch, sizeof(ch), fp);
    fclose(fp);
    long user = 0L, nice = 0L, sys = 0L, idle = 0L, iowait = 0L, irq = 0L, softirq = 0L, usage = 0L;
    sscanf(ch, "%s%ld%ld%ld%ld%ld%ld%ld", cpu, &user, &nice, &sys, &idle, &iowait, &irq, &softirq);
    tt = user + nice + sys + idle + iowait + irq + softirq;
    if (i > 0)
        usage = ((tt - tt0) - (idle - idle0)) * 100 / (tt - tt0);
    //printf("%s,%d,%d,%d,%d,%d,%d,%d\n",cpu,user,nice,sys,idle,iowait,irq,softirq);
    sprintf(cpusage, "%ld%%", usage);
    idle0 = idle;
    tt0 = tt;
    i++;
    return cpusage;
}

void netdev()
{
    FILE *fp = NULL;
    char ch[150] = "", ch4[150] = "", itf[10] = "";
    int r2 = 0, r3 = 0, r4 = 0, r5 = 0, r6 = 0, r7 = 0, r8 = 0, t2 = 0, t3 = 0, t4 = 0, t5 = 0, t6 = 0, t7 = 0, t8 = 0;
    fp = fopen("/proc/net/dev", "r");
    fgets(ch, sizeof(ch), fp);
    fgets(ch, sizeof(ch), fp);
    fgets(ch, sizeof(ch), fp);
    fgets(ch4, sizeof(ch4), fp);
    fclose(fp);
    //printf("%s\n",ch4);
    sscanf(ch4, "%s%lld%d%d%d%d%d%d%d%lld%d%d%d%d%d%d%d", itf, &r1, &r2, &r3, &r4, &r5, &r6, &r7, &r8, &t1, &t2, &t3, &t4, &t5, &t6, &t7, &t8);
    //printf("%d,%d\n",r1,t1);
}

char *B2G(long long b)
{
    static char g[10] = "";
    if (b > 999999999)
    {
        sprintf(g, "%.2fGB", (float)b / 1073741824);
    }
    else
    {
        if (b > 999999)
        {
            sprintf(g, "%.2fMB", (float)b / 1048576);
        }
        else
        {
            if (b > 999)
            {
                sprintf(g, "%.2fKB", (float)b / 1024);
            }
            else
            {
                sprintf(g, "%lldB", b);
            }
        }
    }
    return g;
}

gint settime(gpointer data)
{
    char title[128] = "", label1[1024] = "", hms[10] = "", mems[100] = "", cpusage[20] = "", cr[10] = "", ct[10] = "", crs[10] = "", cts[10] = "", cmu[10] = "", cmt[10] = "";
    int rs = 0, ts = 0, memusage = 0;
    uptime(hms);
    meminfo();
    cpustat(cpusage);
    netdev();
    rs = r1 - r0;
    ts = t1 - t0;
    mu = mt - mf;
    memusage = mu * 100 / mt;
    strcpy(cmt, B2G(mt * 1024));
    strcpy(cmu, B2G(mu * 1024));
    strcpy(cr, B2G(r1));
    strcpy(ct, B2G(t1));
    strcpy(crs, B2G(rs));
    strcpy(cts, B2G(ts));
    sprintf(label1, "Duration: %s\nCPU: %s\nMem: %s/%s=%d%%\nDown: %s , %s/s\nUp: %s , %s/s", hms, cpusage, cmu, cmt, memusage, cr, crs, ct, cts);
    sprintf(title, "C: %s  M: %d%%\nD:%s/s U:%s/s", cpusage, memusage, crs, cts);
    gtk_label_set_text(GTK_LABEL(label), label1);
    gtk_window_set_title(GTK_WINDOW(window), title);
    r0 = r1;
    t0 = t1;
    return TRUE;
}

int main(int argc, char *argv[])
{
    gtk_init(&argc, &argv);
    window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 100);
    gtk_window_set_title(GTK_WINDOW(window), "CMDU");
    //gtk_window_set_decorated(GTK_WINDOW(window), FALSE);
    gtk_window_set_keep_above(GTK_WINDOW(window), TRUE);
    gtk_window_set_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);

    label = gtk_label_new("");
    //gtk_widget_set_opacity(GTK_WIDGET(label), 0.7);
    gtk_label_set_text(GTK_LABEL(label), "Duration:\nCPU:\nMem:\nDown:\nUp:");
    gtk_container_add(GTK_CONTAINER(window), label);

    gint s = g_timeout_add(1000, settime, NULL);
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_widget_show_all(window);
    gtk_main();

    return 0;
}
