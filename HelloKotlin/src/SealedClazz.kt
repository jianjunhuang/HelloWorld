sealed class Expr

data class Const(val number: Int) : Expr()
data class Sum(val s1: Int, val s2: Int) : Expr()
object NotNumber : Expr()

//编译时会提醒你列出所有分支
fun test(expr: Expr): Int = when (expr) {
    is Const -> expr.number
    is Sum -> expr.s1 + expr.s2
    NotNumber -> 0
}
