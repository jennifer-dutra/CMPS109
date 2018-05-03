public class MSDStringSort {
    static int R = 2<<8;

    public static void sort(String[] s){
        String[] aux = new String[s.length];
        int lo = 0, hi = s.length-1, at = 0;
        sort(s, aux, lo, hi, at);
    }

    private static int charAt(String s, int i){
        if(i<s.length())return s.charAt(i);
        else return -1;
    }

    private static void sort(String[] s, String[] aux, int lo, int hi, int at){

        if(hi<=lo)return;

        int[] count = new int[R+2];

        for(int i = lo; i <= hi; ++i)    count[charAt(s[i], at)+2]++;

        for(int i = 0; i < R+1; ++i)     count[i+1] += count[i];

        for(int i = lo; i <= hi; ++i)    aux[count[charAt(s[i], at)+1]++] = s[i];

        for(int i = lo; i <= hi; ++i)    s[i] = aux[i-lo];

        for(int r=0;r<R;++r) sort(s, aux, lo+count[r], lo+count[r+1]-1, at+1);
    }

    public static void main(String[] args) throws Exception{

        // String[] s = {"a", "aa", "ab", "abc", "d", "qq", "ff"};
        String[] s = { "33", "54", "3", "135", "644", "3", "5", "13", "53", "502", "99" };
        sort(s);
        for(int i=0;i<s.length;++i) System.out.println(s[i]);
    }
}
