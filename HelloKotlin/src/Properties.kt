var allByDefault: Int? = null
    // 错误：需要显式初始化器，隐含默认 getter 和 setter
    get() = 1
    set(value) {
        field = value
    }
var initialized = 1 // 类型 Int、默认 getter 和 setter