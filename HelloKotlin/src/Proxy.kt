class CountingSet<T>(val innerSet: MutableCollection<T> = HashSet<T>()) : MutableCollection<T> by innerSet {

    override fun add(element: T): Boolean {
        return innerSet.add(element)
    }

}