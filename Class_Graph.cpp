#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <iomanip>
#include <cassert>
#include <sstream>
#include <algorithm>
#include "Class_Graph.h"

using namespace std;

/// Тестирование операций с вершинами (AddV, DeleteV, HasVert, Size)
void test_vertex_operations() {
    // Используем INF = 1000000000
    DGraph<string, double> g(1000000000);

    // пустой граф
    assert(g.Size() == 0);
    assert(!g.HasVert("A"));

    // добавление одной вершины
    g.AddV("A");
    assert(g.Size() == 1);
    assert(g.HasVert("A"));

    // добавление нескольких вершин
    g.AddV("B");
    g.AddV("C");
    assert(g.Size() == 3);
    assert(g.HasVert("B") && g.HasVert("C"));

    // попытка добавить уже существующую вершину выбрасывает исключение
    try {
        g.AddV("B");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // удаление вершины в середине
    g.DeleteV("B");
    assert(g.Size() == 2);
    assert(!g.HasVert("B"));
    // другие вершины сохранились
    assert(g.HasVert("A") && g.HasVert("C"));

}

/// Тестирование операций с рёбрами (AddE, DeleteE, HasEdge, GetEdge)
void test_edge_operations() {
    DGraph<string, double> g(1000000000);

    // пустой граф — попытка добавить ребро = исключение (вершин нет)
    try {
        g.AddE("A", "B", 1.0);
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // создаём вершины, проверяем, что ребра отсутствуют
    g.AddV("A");
    g.AddV("B");
    g.AddV("C");
    assert(!g.HasEdge("A", "B"));
    assert(!g.HasEdge("A", "C"));

    // добавляем ребро A->B
    g.AddE("A", "B", 2.5);
    assert(g.HasEdge("A", "B"));
    assert(g.GetEdge("A", "B") == 2.5);

    // редактируем ребро 
    g.AddE("A", "B", 7.0);
    assert(g.GetEdge("A", "B") == 7.0);

    // удаляем ребро
    g.DeleteE("A", "B");
    assert(!g.HasEdge("A", "B"));

    // попытка получить несуществующее ребро бросает исключение
    try {
        g.GetEdge("A", "B");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // операции с отсутствующей вершиной должны бросать исключение
    try {
        g.DeleteE("X", "Y");
        assert(false);
    }
    catch (const runtime_error&) {

    }
}

/// Тест BFS
void test_bfs() {
    DGraph<string, double> g(1000000000);

    // пустой граф = запуск BFS должен бросить исключение
    try {
        g.BFS("A");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // один узел
    g.AddV("A");
    auto order1 = g.BFS("A");
    assert(order1.size() == 1 && order1[0] == "A");

    // несколько вершин, но без рёбер
    g.AddV("B");
    g.AddV("C");
    auto order2 = g.BFS("A");
    // BFS должен вернуть только стартовую вершину, т.к. нет рёбер
    assert(order2.size() == 1 && order2[0] == "A");

    // простой путь A->B->C
    g.AddE("A", "B", 1.0);
    g.AddE("B", "C", 1.0);
    auto order3 = g.BFS("A");
    // Oжидаемый порядок: A, B, C
    assert(order3.size() == 3 && order3[0] == "A" && order3[1] == "B" && order3[2] == "C");

    // ветвящийся граф A->B, A->C, B->D, C->E
    g.AddV("D");
    g.AddV("E");
    g.AddE("A", "C", 1.0);
    g.AddE("B", "D", 1.0);
    g.AddE("C", "E", 1.0);
    auto order4 = g.BFS("A");
    // BFS посетит уровни: A, (B,C), (D,E) — порядок между B и C зависит от индексов вершин.
    assert(order4.size() == 5);
    assert(order4[0] == "A");
    assert(order4[1] == "B");
    assert(order4[2] == "C");
    assert(order4[3] == "D");
    assert(order4[4] == "E");

    // граф с циклом (A->B, B->A)
    DGraph<string, double> g2(1000000000);
    g2.AddV("A");
    g2.AddV("B");
    g2.AddE("A", "B", 1.0);
    g2.AddE("B", "A", 1.0);
    auto order5 = g2.BFS("A");
    // BFS должен посетить A и B ровно по одному разу
    assert(order5.size() == 2);
    assert(order5[0] == "A" && order5[1] == "B");
}

/// Тест DFS
void test_dfs() {
    DGraph<string, double> g(1000000000);

    // пустой граф = ошибка
    try {
        g.DFS("A");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // один узел
    g.AddV("A");
    auto o1 = g.DFS("A");
    assert(o1.size() == 1 && o1[0] == "A");

    // несколько вершин без рёбер
    g.AddV("B");
    g.AddV("C");
    auto o2 = g.DFS("A");
    assert(o2.size() == 1 && o2[0] == "A");

    // простой путь A->B->C
    g.AddE("A", "B", 1.0);
    g.AddE("B", "C", 1.0);
    auto o3 = g.DFS("A");
    // A, B, C
    assert(o3.size() == 3 && o3[0] == "A" && o3[1] == "B" && o3[2] == "C");

    // ветвящийся граф
    g.AddV("D");
    g.AddV("E");
    g.AddE("A", "C", 1.0);
    g.AddE("B", "D", 1.0);
    g.AddE("C", "E", 1.0);
    auto o4 = g.DFS("A");
    assert(o4.size() == 5);
    assert(o4[0] == "A");
    assert(o4[1] == "C");
    assert(o4[2] == "E");
    assert(o4[3] == "B");
    assert(o4[4] == "D");

    // граф с циклом A->B, B->A
    DGraph<string, double> g2(1000000000);
    g2.AddV("A");
    g2.AddV("B");
    g2.AddE("A", "B", 1.0);
    g2.AddE("B", "A", 1.0);
    auto o5 = g2.DFS("A");
    assert(o5.size() == 2);
}

/// Тест GetNeighbors
void test_get_neighbors() {

    const double INF = 1000000000;
    DGraph<string, double> g(INF);

    // пустой граф = GetNeighbors должен бросить исключение (вершины нет)
    try {
        auto r = g.GetNeighbors("A");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // граф из одной вершины -> neighbors пуст
    g.AddV("A");
    {
        auto n = g.GetNeighbors("A");
        assert(n.empty());
    }

    // несколько вершин, но нет рёбер -> neighbors пуст
    g.AddV("B");
    g.AddV("C");
    {
        auto n = g.GetNeighbors("A");
        assert(n.empty());
        // Также проверить для другой вершины
        auto nb = g.GetNeighbors("B");
        assert(nb.empty());
    }

    // большой разреженный граф 
    // добавим 200 вершин, установим ребра 0->1..10
    DGraph<int, double> big(INF);
    const int N = 200;
    for (int i = 0; i < N; ++i)
        big.AddV(i);
    for (int i = 1; i <= 10; ++i)
        big.AddE(0, i, 1.0 * i);

    {
        auto neigh0 = big.GetNeighbors(0);
        // Ожидаем ровно 10 соседей: 1..10
        assert(neigh0.size() == 10);
        for (int i = 1; i <= 10; ++i) {
            // в big вершины представлены как int, поэтому проверяем присутствие
            assert(find(neigh0.begin(), neigh0.end(), i) != neigh0.end());
        }
        // Уверимся, что почти все другие вершины не имеют соседей (например, 50)
        auto neigh50 = big.GetNeighbors(50);
        assert(neigh50.empty());
    }
}

/// Тест GetWeight
void test_get_weight() {

    const double INF = 1000000000;
    DGraph<string, double> g(INF);

    // пустой граф = GetWeight должен бросить исключение
    try {
        double w = g.GetWeight("A", "B");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // граф из одной вершины = GetWeight(A,A) должно бросать (ребро отсутствует)
    g.AddV("A");
    try {
        double w = g.GetWeight("A", "A");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // простое направленное ребро A->B
    g.AddV("B");
    g.AddE("A", "B", 3.5);
    {
        double w = g.GetWeight("A", "B");
        assert(w == 3.5);
    }

    // есть только обратное ребро C->A
    g.AddV("C");
    g.AddE("C", "A", 2.0);
    // GetWeight(A,C) — нет ребра A->C = бросит исключение
    try {
        double w = g.GetWeight("A", "C");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // GetWeight(C,A) вернёт 2.0
    {
        double w = g.GetWeight("C", "A");
        assert(w == 2.0);
    }

    // двунаправленное ребро X <-> Y: веса различны
    g.AddV("X");
    g.AddV("Y");
    g.AddE("X", "Y", 1.0);
    g.AddE("Y", "X", 4.0);
    {
        double wxy = g.GetWeight("X", "Y");
        double wyx = g.GetWeight("Y", "X");
        assert(wxy == 1.0 && wyx == 4.0);
    }

    // попытка GetWeight для несуществующей вершины = бросит ислючение
    try {
        g.GetWeight("test", "A");
        assert(false);
    }
    catch (const runtime_error&) {

    }

    // большой граф: проверим корректность для цепочки
    DGraph<int, double> big(INF);
    const int M = 100;
    for (int i = 0; i < M; ++i)
        big.AddV(i);
    for (int i = 0; i < M - 1; ++i)
        big.AddE(i, i + 1, (double)i + 0.5);

    // проверка нескольких значений
    for (int i = 0; i < 99; ++i) {
        double w = big.GetWeight(i, i + 1);
        assert(w == (double)i + 0.5);
    }
}

/// Тест BellmanFord
void test_bellman_ford() {
    const double INF = 1000000000;


    // пустой граф = вызов с несуществующей вершиной должен бросить исключение
    {
        DGraph<string, double> g(INF);
        try {
            auto res = g.BellmanFord("A");
            assert(false);
        }
        catch (const runtime_error&) {

        }
    }

    // граф из одной вершины — расстояние до себя = 0
    {
        DGraph<string, double> g(INF);
        g.AddV("A");
        auto res = g.BellmanFord("A");
        // должно быть ровно одна вершина и расстояние 0
        assert(res.size() == 1);
        assert(res[0] == 0.0);
    }

    // несколько вершин, но нет рёбер — только стартовая 0, остальные INF
    {
        DGraph<string, double> g(INF);
        g.AddV("A");
        g.AddV("B");
        g.AddV("C");
        // нет рёбер
        auto res = g.BellmanFord("A");
        assert(res.size() == 3);
        // проверяем: индекс 0 -> "A" (добавлена первой) = 0
        assert(res[0] == 0.0);
        // остальные должны быть недостижимы (INF)
        assert((res[1]) == INF);
        assert((res[2]) == INF);
    }

    // отрицательные веса, но без отрицательного цикла
    // Пример: A->B:5, A->C:2, C->B:-3 => A->B через C = -1
    {
        DGraph<string, double> g(INF);
        g.AddV("A");
        g.AddV("B");
        g.AddV("C");
        g.AddE("A", "B", 5.0);
        g.AddE("A", "C", 2.0);
        g.AddE("C", "B", -3.0);
        auto res = g.BellmanFord("A");
        // Ожидаем: A = 0, C = 2, B = -1
        assert(res.size() == 3);
        assert(res[0] == 0.0);    // A
        assert(res[2] == 2.0);    // C
        assert(res[1] == -1.0);   // B через C: -1
    }

    // обнаружение отрицательного цикла = метод должен бросить исключение
    {
        DGraph<string, double> g(INF);
        g.AddV("A");
        g.AddV("B");
        g.AddV("C");
        g.AddE("A", "B", 1.0);
        g.AddE("B", "C", -2.0);
        g.AddE("C", "A", -1.0);
        try {
            auto res = g.BellmanFord("A");
            assert(false);
        }
        catch (const runtime_error&) {

        }
    }

    // смесь достижимых и недостижимых вершин 
    {
        DGraph<int, double> g(INF);
        g.AddV(0);
        g.AddV(1);
        g.AddV(2);
        g.AddV(3);
        g.AddE(0, 1, 1.0);
        g.AddE(1, 2, 1.0);
        // вершина 3 изолирована
        auto res = g.BellmanFord(0);
        assert(res.size() == 4);
        assert(res[0] == 0.0);   // старт
        assert(res[1] == 1.0);
        assert(res[2] == 2.0);
        assert((res[3]) == INF); // недостижима
    }

    // большой граф — цепочка из N вершин 
    {
        DGraph<int, double> g(INF);
        const int N = 200;
        for (int i = 0; i < N; ++i)
            g.AddV(i);
        for (int i = 0; i < N - 1; ++i)
            g.AddE(i, i + 1, 1.0);
        auto res = g.BellmanFord(0);
        assert(res.size() == N);
        // проверяем несколько точек
        assert(res[0] == 0.0);
        assert(res[1] == 1.0);
        assert(res[10] == 10.0);
        assert(res[N - 1] == double(N - 1));
    }
}

/// Тест saveGraphML
void test_saveGraphML() {
    const double INF = 1000000000;

    // пустой граф 
    {
        string fname = "test_save_empty.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        g.saveGraphML(fname);

        // Файл должен существовать и содержать graphml header
        ifstream f(fname);
        assert(f.is_open());
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        assert(content.find("<graphml") != string::npos);
        assert(content.find("<graph") != string::npos);
    }

    // один узел 
    {
        string fname = "test_save_one_node.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        g.AddV("A");
        g.saveGraphML(fname);

        ifstream f(fname);
        assert(f.is_open());
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        // Должен присутствовать node n0 и label A
        assert(content.find("node id=\"n0\"") != string::npos);
        assert(content.find("A") != string::npos);
    }

    // несколько вершин без рёбер 
    {
        string fname = "test_save_nodes_no_edges.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        g.AddV("A");
        g.AddV("B");
        g.AddV("C");
        g.saveGraphML(fname);

        ifstream f(fname);
        assert(f.is_open());
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        // Проверим количество появлений "<node"
        size_t pos = 0;
        int nodeCount = 0;
        while ((pos = content.find("<node", pos)) != string::npos) {
            ++nodeCount;
            pos += 5;
        }
        assert(nodeCount == 3);

        // В файле не должно быть тегов <edge> (поскольку нет рёбер)
        assert(content.find("<edge") == string::npos);
    }

    // граф с рёбрами и весами 
    {
        string fname = "test_save_edges.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        g.AddV("A");
        g.AddV("B");
        g.AddE("A", "B", 5.5);
        g.saveGraphML(fname);

        ifstream f(fname);
        assert(f.is_open());
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        // Ожидаем наличие edge с source n0 target n1 и вес 5.5
        assert(content.find("<edge") != string::npos);
        assert(content.find("source=\"n0\"") != string::npos);
        assert(content.find("target=\"n1\"") != string::npos);
        assert(content.find("5.5") != string::npos);
    }

    // большой граф 
    {
        string fname = "test_save_large.graphml";
        remove(fname.c_str());

        DGraph<int, double> g(INF);
        const int N = 100;
        for (int i = 0; i < N; ++i)
            g.AddV(i);
        for (int i = 0; i < N - 1; ++i)
            g.AddE(i, i + 1, 1.0);
        g.saveGraphML(fname);

        // Открываем файл обычным способом
        ifstream f(fname);
        assert(f.is_open());

        // Читаем содержимое в строку
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());

        f.close();

        // Проверка размера 
        assert(content.length() > 1000);

        //  Проверки узлов 
        assert(content.find("<node id=\"n0\"") != string::npos);
        assert(content.find("<node id=\"n10\"") != string::npos);
        assert(content.find("<node id=\"n50\"") != string::npos);
        assert(content.find("<node id=\"n99\"") != string::npos);

        //  Проверки рёбер 
        assert(content.find("<edge isDirected=\"true\" source=\"n0\" target=\"n1\"") != string::npos);
        assert(content.find("<edge isDirected=\"true\" source=\"n10\" target=\"n11\"") != string::npos);
        assert(content.find("<edge isDirected=\"true\" source=\"n50\" target=\"n51\"") != string::npos);
        assert(content.find("<edge isDirected=\"true\" source=\"n98\" target=\"n99\"") != string::npos);
    }


    // перезапись файла (save дважды) 
    {
        string fname = "test_save_overwrite.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        g.AddV("X");
        g.saveGraphML(fname);

        // Сохраняем другой граф в тот же файл
        DGraph<string, double> g2(INF);
        g2.AddV("Y");
        g2.saveGraphML(fname);

        ifstream f(fname);
        assert(f.is_open());
        string content((istreambuf_iterator<char>(f)), istreambuf_iterator<char>());
        f.close();

        // Проверяем, что записался именно новый граф
        assert(content.find("name = \"Y\"") != string::npos);
        assert(content.find("name = \"X\"") == string::npos);

    }
}

/// Тест loadGraphML
void test_loadGraphML() {
    const double INF = 1000000000;

    // несуществующий файл =ошибка)
    {
        string fname = "this_file_does_not_exist.graphml";
        remove(fname.c_str());

        DGraph<string, double> g(INF);
        try {
            g.loadGraphML(fname);
            assert(false);
        }
        catch (const runtime_error&) {

        }
    }

    // пустой файл 
    {
        string fname = "test_load_empty.graphml";
        {
            ofstream f(fname);
        }

        DGraph<string, double> g(INF);
        g.loadGraphML(fname);
        assert(g.Size() == 0);

        remove(fname.c_str());
    }

    // один узел 
    {
        string fname = "test_load_one_node.graphml";
        ofstream f(fname);
        f << "<?xml version=\"1.0\"?>\n";
        f << "<graphml>\n";
        f << "  <graph id=\"G\" edgedefault=\"directed\">\n";
        f << "    <node id=\"n0\" name=\"A\"></node>\n";
        f << "  </graph>\n";
        f << "</graphml>\n";
        f.close();

        DGraph<string, double> g(INF);
        g.loadGraphML(fname);

        assert(g.Size() == 1);
        assert(g.HasVert("A"));
        remove(fname.c_str());
    }

    // несколько вершин без рёбер  
    {
        string fname = "test_load_nodes_no_edges.graphml";
        ofstream f(fname);
        f << "<?xml version=\"1.0\"?>\n<graphml>\n  <graph id=\"G\" edgedefault=\"directed\">\n";
        for (int i = 0; i < 4; ++i) {
            f << "    <node id=\"n" << i << "\" name=\"V" << i << "\"></node>\n";
        }
        f << "  </graph>\n</graphml>\n";
        f.close();

        DGraph<string, double> g(INF);
        g.loadGraphML(fname);

        assert(g.Size() == 4);
        for (int i = 0; i < 4; ++i) {
            assert(g.HasVert("V" + to_string(i)));
        }
        assert(!g.HasEdge("V0", "V1"));
        remove(fname.c_str());
    }

    // файл с рёбрами и весами 
    {
        string fname = "test_load_edges.graphml";
        ofstream f(fname);
        f << "<?xml version=\"1.0\"?>\n<graphml>\n";
        f << "  <graph id=\"G\" edgedefault=\"directed\">\n";
        f << "    <node id=\"n0\" name=\"A\"></node>\n";
        f << "    <node id=\"n1\" name=\"B\"></node>\n";
        f << "    <edge isDirected=\"true\" source=\"n0\" target=\"n1\" weight=\"5.5\"></edge>\n";
        f << "  </graph>\n</graphml>\n";
        f.close();

        DGraph<string, double> g(INF);
        g.loadGraphML(fname);

        assert(g.Size() == 2);
        assert(g.HasVert("A"));
        assert(g.HasVert("B"));
        assert(g.HasEdge("A", "B"));
        assert(g.GetWeight("A", "B") == 5.5);

        remove(fname.c_str());
    }

    // большой файл 
    {
        string fname = "test_load_large.graphml";
        ofstream f(fname);
        f << "<?xml version=\"1.0\"?>\n<graphml>\n  <graph id=\"G\" edgedefault=\"directed\">\n";
        const int N = 80;

        for (int i = 0; i < N; ++i)
            f << "    <node id=\"n" << i << "\" name=\"n" << i << "\"></node>\n";

        for (int i = 0; i < N - 1; ++i)
            f << "    <edge isDirected=\"true\" source=\"n" << i << "\" target=\"n" << (i + 1)
            << "\" weight=\"1.0\"></edge>\n";

        f << "  </graph>\n</graphml>\n";
        f.close();

        DGraph<string, double> g(INF);
        g.loadGraphML(fname);

        assert(g.Size() == N);

        assert(g.HasVert("n0"));
        assert(g.HasVert("n10"));

        assert(g.HasEdge("n0", "n1"));
        assert(g.GetWeight("n0", "n1") == 1.0);

        remove(fname.c_str());
    }
}