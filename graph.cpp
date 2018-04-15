#include "graph.h"

/***************************************************
                    VERTEX
****************************************************/

/// Le constructeur met en place les éléments de l'interface
VertexInterface::VertexInterface(int idx, int x, int y, std::string pic_name, int pic_idx)
{
    // La boite englobante
    m_top_box.set_pos(x, y);
    m_top_box.set_dim(130, 100);
    m_top_box.set_moveable();

    // Le slider de réglage de valeur
    m_top_box.add_child( m_slider_value );
    m_slider_value.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_value.set_dim(20,80);
    m_slider_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);

    // Label de visualisation de valeur
    m_top_box.add_child( m_label_value );
    m_label_value.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);

    // Une illustration...
    if (pic_name!="")
    {
        m_top_box.add_child( m_img );
        m_img.set_pic_name(pic_name);
        m_img.set_pic_idx(pic_idx);
        m_img.set_gravity_x(grman::GravityX::Right);
    }

    // Label de visualisation d'index du sommet dans une boite
    m_top_box.add_child( m_box_label_idx );
    m_box_label_idx.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Down);
    m_box_label_idx.set_dim(20,12);
    m_box_label_idx.set_bg_color(BLANC);

    m_box_label_idx.add_child( m_label_idx );
    m_label_idx.set_message( std::to_string(idx) );

    m_top_box.add_child( m_supp );
    m_supp.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_supp.set_dim(10,10);
    m_supp.set_bg_color(ROUGE);

    m_supp.add_child(m_t4);
    m_t4.set_message("X");


}

/// Gestion du Vertex avant l'appel à l'interface
void Vertex::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_value vers le slider associé
    m_interface->m_slider_value.set_value(m_value);

    /// Copier la valeur locale de la donnée m_value vers le label sous le slider
    m_interface->m_label_value.set_message( std::to_string( (int)m_value) );
}


/// Gestion du Vertex après l'appel à l'interface
void Vertex::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_value locale
    m_value = m_interface->m_slider_value.get_value();
}



/***************************************************
                    EDGE
****************************************************/

/// Le constructeur met en place les éléments de l'interface
EdgeInterface::EdgeInterface(Vertex& from, Vertex& to)
{
    // Le WidgetEdge de l'interface de l'arc
    if ( !(from.m_interface && to.m_interface) )
    {
        std::cerr << "Error creating EdgeInterface between vertices having no interface" << std::endl;
        throw "Bad EdgeInterface instanciation";
    }
    m_top_edge.attach_from(from.m_interface->m_top_box);
    m_top_edge.attach_to(to.m_interface->m_top_box);
    m_top_edge.reset_arrow_with_bullet();

    // Une boite pour englober les widgets de réglage associés
    m_top_edge.add_child(m_box_edge);
    m_box_edge.set_dim(24,60);
    m_box_edge.set_bg_color(BLANCBLEU);

    // Le slider de réglage de valeur
    m_box_edge.add_child( m_slider_weight );
    m_slider_weight.set_range(0.0 , 100.0); // Valeurs arbitraires, à adapter...
    m_slider_weight.set_dim(16,40);
    m_slider_weight.set_gravity_y(grman::GravityY::Up);

    // Label de visualisation de valeur
    m_box_edge.add_child( m_label_weight );
    m_label_weight.set_gravity_y(grman::GravityY::Down);

}


/// Gestion du Edge avant l'appel à l'interface
void Edge::pre_update()
{
    if (!m_interface)
        return;

    /// Copier la valeur locale de la donnée m_weight vers le slider associé
    m_interface->m_slider_weight.set_value(m_weight);

    /// Copier la valeur locale de la donnée m_weight vers le label sous le slider
    m_interface->m_label_weight.set_message( std::to_string( (int)m_weight ) );
}

/// Gestion du Edge après l'appel à l'interface
void Edge::post_update()
{
    if (!m_interface)
        return;

    /// Reprendre la valeur du slider dans la donnée m_weight locale
    m_weight = m_interface->m_slider_weight.get_value();
}



/***************************************************
                    GRAPH
****************************************************/

/// Ici le constructeur se contente de préparer un cadre d'accueil des
/// éléments qui seront ensuite ajoutés lors de la mise ne place du Graphe
GraphInterface::GraphInterface(int x, int y, int w, int h)
{
    m_top_box.set_dim(800,600);
    m_top_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);

    m_top_box.add_child(m_tool_box);
    m_tool_box.set_dim(120,600);
    m_tool_box.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_tool_box.set_bg_color(BLANCBLEU);

    m_top_box.add_child(m_main_box);
    m_main_box.set_dim(680,600);
    m_main_box.set_gravity_xy(grman::GravityX::Right, grman::GravityY::Up);
    m_main_box.set_bg_color(BLANCJAUNE);


    // AJOUTER
    m_tool_box.add_child(m_box_ajouter);
    m_box_ajouter.set_dim(110,110);
    m_box_ajouter.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Up);
    m_box_ajouter.set_bg_color(ROUGECLAIR);

    m_box_ajouter.add_child(m_b1);
    m_b1.set_frame(15,15,80,80);
    m_b1.set_bg_color(ROUGESOMBRE);

    m_b1.add_child(m_t1);
    m_t1.set_message("AJOUTER SOMMET");

    //SAUVER
    m_tool_box.add_child(m_box_sauver);
    m_box_sauver.set_dim(110,110);
    m_box_sauver.set_pos(0,225);
    m_box_sauver.set_bg_color(VERTCLAIR);

    m_box_sauver.add_child(m_b2);
    m_b2.set_frame(15,15,80,80);
    m_b2.set_bg_color(VERT);

    m_b2.add_child(m_t2);
    m_t2.set_message("SAUVER");

    //MENU
    m_tool_box.add_child(m_box_menu);
    m_box_menu.set_dim(110,110);
    m_box_menu.set_gravity_xy(grman::GravityX::Left, grman::GravityY::Down);
    m_box_menu.set_bg_color(JAUNECLAIR);

    m_box_menu.add_child(m_b5);
    m_b5.set_frame(15,15,80,80);
    m_b5.set_bg_color(JAUNE);

    m_b5.add_child(m_t5);
    m_t5.set_message("MENU");

    // AJOUTER ARRETE
    m_tool_box.add_child(m_box_ajouter2);
    m_box_ajouter2.set_dim(110,110);
    m_box_ajouter2.set_pos(0,115);
    m_box_ajouter2.set_bg_color(ORANGECLAIR);

    m_box_ajouter2.add_child(m_b6);
    m_b6.set_frame(15,15,80,80);
    m_b6.set_bg_color(ORANGE);

    m_b6.add_child(m_t6);
    m_t6.set_message("AJOUTER ARETE");

    // CONNEXITE
    m_tool_box.add_child(m_box_connexite);
    m_box_connexite.set_dim(110,110);
    m_box_connexite.set_pos(0,335);
    m_box_connexite.set_bg_color(BLEUCLAIR);

    m_box_connexite.add_child(m_b7);
    m_b7.set_frame(15,15,80,80);
    m_b7.set_bg_color(BLEU);

    m_b7.add_child(m_t7);
    m_t7.set_message("CONNEXITE");
}

void Graph::affichage_adjacence()
{
    std::cout << std::endl << "Matrice d'adjacence :" << std::endl;
    for(auto& elem : m_adjacence)
    {
        for(auto& elmt : elem)
            std::cout << elmt << "  ";
        std::cout << std::endl;
    }
}

void Graph::ajouter_sommet()
{
    std::string name;
    std::vector<int> vec;
    int n;
    std::cout << "Nom de l'animal a ajouter : ";
    std::cin >> name;
    std::cout << "Numero du sommet : ";
    std::cin >> n;
    name += ".jpg";
    add_interfaced_vertex(n,0.0,400,300,name);
    //Agrandir matrice d'adjacence
    for(auto& elem : m_adjacence)
        elem.push_back(0);
    for(int i=0; i<m_vertices.size(); i++)
        vec.push_back(0);
    m_adjacence.push_back(vec);

    affichage_adjacence();
}

void Graph::ajouter_arete()
{
    int i, j;
    std::cout << "Sommet 1 : ";
    std::cin >> i;
    std::cout << "Sommet 2 : ";
    std::cin >> j;
    add_interfaced_edge(m_nbEdges,i,j,0.0);
    m_nbEdges++;
    //Mettre a jour matrice d'adjacence
    m_adjacence[i][j]=1;
    affichage_adjacence();
}

void Graph::sauvegarder()
{
    std::ofstream fic(m_nom.c_str());
    if(fic)
    {
        fic << m_vertices.size() << std::endl;
        for(auto& elem : m_vertices)
            fic << elem.first << " " << elem.second.m_value << " " << elem.second.m_interface->m_top_box.get_posx() << " " << elem.second.m_interface->m_top_box.get_posy() << " " << elem.second.m_interface->m_img.getPicName() << std::endl;

        fic << m_edges.size() << std::endl;
        for(auto& elem : m_edges)
            fic << elem.second.m_from << " " << elem.second.m_to << " " << elem.second.m_weight << std::endl;

        std::cout << "Le fichier " << m_nom << " a bien ete sauvegarde" << std::endl;
    }
    else
        std::cerr << "Erreur creation du fichier : " << m_nom << std::endl;
}

/// Chargement du graphe depuis un fichier
void Graph::make_example(std::string nom)
{
    m_interface = std::make_shared<GraphInterface>(50, 0, 750, 600);
    // La ligne précédente est en gros équivalente à :
    // m_interface = new GraphInterface(50, 0, 750, 600);

    m_nom = nom + ".txt";
    //Variables locales
    int nbSommets, nbAretes;
    int n, x, y;
    double val;
    std::string name;
    std::vector<int> vec;

    //Ouverture du fichier "m_nom" en mode lecture
    std::ifstream fic(m_nom.c_str());
    if(fic)
    {
        std::cout << "Chargement du fichier " << m_nom << std::endl;
        //Initialisation de la map de sommets
        fic >> nbSommets;
        for(int i=0; i<nbSommets; i++)
        {
            fic >> n >> val >> x >> y >> name;
            add_interfaced_vertex(n, val, x, y, name);
            //initialisation de la matrice d'adjacence
            vec.push_back(0);
        }
        for(int j=0; j<nbSommets; j++)
            m_adjacence.push_back(vec);

        //Initialisation de la map d'aretes
        fic >> nbAretes;
        for(int i=0; i<nbAretes; i++)
        {
            fic >> x >> y >> val;
            add_interfaced_edge(i, x, y, val);
            //Initialisation des sommets adjacents
            m_adjacence[x][y] = 1;
        }
        m_nbEdges=nbAretes;

        //Affichage matrice d'adjacence
        affichage_adjacence();

        //Fermeture du fichier
        fic.close();
    }
    else
        std::cerr << "Erreur d'ouverture du fichier : " << m_nom << std::endl;
}

/// La méthode update à appeler dans la boucle de jeu pour les graphes avec interface
void Graph::update()
{

    //FONCTON BOUTTON
    if (!m_interface)
        return;

    for (auto &elt : m_vertices)
        elt.second.pre_update();

    for (auto &elt : m_edges)
        elt.second.pre_update();

    m_interface->m_top_box.update();


    //SUPPRIMER SOMMET
    for (auto &elt : m_vertices)
    {
        elt.second.post_update();
        if(elt.second.m_interface->m_supp.get_value())
        {
            std::cout << std::endl << "Removing vertex " << elt.first << " " << elt.second.m_interface->m_img.getPicName() << std::endl;
            test_remove_vertex(elt.first);
            elt.second.m_interface->m_supp.set_value(false);
        }
    }

    //MENU
    if(m_interface->m_b5.get_value())
    {
        std::cout << std::endl << "menu" << std::endl;
        afficher_choix();

        m_interface->m_b5.set_value(false);
    }

    //SAUVEGARDER
    if(m_interface->m_b2.get_value())
    {
        sauvegarder();
        m_interface->m_b2.set_value(false);
    }

    //AJOUTER SOMMET
    if(m_interface->m_b1.get_value())
    {
        ajouter_sommet();
        m_interface->m_b1.set_value(false);
    }

    //AJOUTER ARETE
    if(m_interface->m_b6.get_value())
    {
        ajouter_arete();
        m_interface->m_b6.set_value(false);
    }

    //CONNEXITE
    if(m_interface->m_b7.get_value())
    {
        ToutesLesComposantesFortementsConnexes();
        m_interface->m_b7.set_value(false);
    }

    for (auto &elt : m_edges)
        elt.second.post_update();

}

void Graph::afficher_choix()
{
    int i;

    clear_bitmap(screen);
    BITMAP* buffer=NULL;
    buffer=create_bitmap(800,600);


    while(!key[KEY_SPACE])
    {
        grman::show_picture(buffer, "choix.bmp", 0, 0, 0);
        if(mouse_x>50 && mouse_y>150 && mouse_x<200 && mouse_y<500)
        {
            rect(buffer, 15, 150, 200, 500, 255);
            i=1;
            if(mouse_b&1)
                lancer("GrapheMarais");
        }

        if(mouse_x>230 && mouse_y>150 && mouse_x<500 && mouse_y<500)
        {
            rect(buffer, 230, 150, 500, 500, 255);
            i=2;
            if(mouse_b&1)
                lancer("GrapheOcean");
        }

        if(mouse_x>505 && mouse_y>150 && mouse_x<790 && mouse_y<500)
        {
            rect(buffer, 505, 150, 790, 500, 255);
            i=3;
            if(mouse_b&1)
                lancer("GrapheSavanne");
        }

        blit(buffer,screen,0,0,0,0,SCREEN_W,SCREEN_H);

    }


}

void Graph::lancer(std::string nom)
{
    grman::set_pictures_path("pics");

    Graph g;
    g.make_example(nom);
//    g.test_remove_edge(1);
//    g.test_remove_edge(2);
//    g.test_remove_edge(3);
//    g.test_remove_edge(4);
//    g.test_remove_edge(5);

//    g.test_remove_vertex(1);
//    g.test_remove_vertex(3);

    while(!key[KEY_SPACE])
    {
        //clear_bitmap(buffer);
        //blit(grman::page,buffer,0,y,0,0,SCREEN_W,SCREEN_H);
        g.update();
        grman::mettre_a_jour();

    }
}


/// Aide à l'ajout de sommets interfacés
void Graph::add_interfaced_vertex(int idx, double value, int x, int y, std::string pic_name, int pic_idx )
{
    if ( m_vertices.find(idx)!=m_vertices.end() )
    {
        std::cerr << "Error adding vertex at idx=" << idx << " already used..." << std::endl;
        throw "Error adding vertex";
    }
    // Création d'une interface de sommet
    VertexInterface *vi = new VertexInterface(idx, x, y, pic_name, pic_idx);
    // Ajout de la top box de l'interface de sommet
    m_interface->m_main_box.add_child(vi->m_top_box);
    // On peut ajouter directement des vertices dans la map avec la notation crochet :
    m_vertices[idx] = Vertex(value, vi);
}

/// Aide à l'ajout d'arcs interfacés
void Graph::add_interfaced_edge(int idx, int id_vert1, int id_vert2, double weight)
{
    if ( m_edges.find(idx)!=m_edges.end() )
    {
        std::cerr << "Error adding edge at idx=" << idx << " already used..." << std::endl;
        throw "Error adding edge";
    }

    if ( m_vertices.find(id_vert1)==m_vertices.end() || m_vertices.find(id_vert2)==m_vertices.end() )
    {
        std::cerr << "Error adding edge idx=" << idx << " between vertices " << id_vert1 << " and " << id_vert2 << " not in m_vertices" << std::endl;
        throw "Error adding edge";
    }

    EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]);
    m_interface->m_main_box.add_child(ei->m_top_edge);
    m_edges[idx] = Edge(weight, ei);

    m_edges[idx].m_from = id_vert1;
    m_edges[idx].m_to = id_vert2;

    m_vertices[id_vert1].m_out.push_back(idx);
    m_vertices[id_vert2].m_in.push_back(idx);
}

void Graph::test_remove_edge(int eidx)
{
/// référence vers le Edge à enlever
    Edge &remed=m_edges.at(eidx);

    std::cout << std::endl << "Removing edge [" << eidx << "] " << remed.m_from << "->" << remed.m_to << " " << remed.m_weight << std::endl;

///// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
//    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
//    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
//    std::cout << m_edges.size() << std::endl;

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_edge );
    }

/// Il reste encore à virer l'arc supprimé de la liste des entrants et sortants des 2 sommets to et from !
/// References sur les listes de edges des sommets from et to
    std::vector<int> &vefrom = m_vertices[remed.m_from].m_out;
    std::vector<int> &veto = m_vertices[remed.m_to].m_in;
    vefrom.erase( std::remove( vefrom.begin(), vefrom.end(), eidx ), vefrom.end() );
    veto.erase( std::remove( veto.begin(), veto.end(), eidx ), veto.end() );

/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_edges.erase( eidx );

///// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;

    //Mise a jour de la matrice d'adjacence
    m_adjacence[remed.m_from][remed.m_to] = 0;
}

void Graph::test_remove_vertex(int eidx)
{
    // variable locale
    std::map<int, Vertex> vertices;
    int i=0;

    for (auto &elt : m_edges)
    {
        if (elt.second.m_from == eidx || elt.second.m_to == eidx)
            test_remove_edge(elt.first);
    }
/// référence vers le Edge à enlever
    Vertex &remed=m_vertices.at(eidx);

/// test : on a bien des éléments interfacés
    if (m_interface && remed.m_interface)
    {
/// Ne pas oublier qu'on a fait ça à l'ajout de l'arc :
        /* EdgeInterface *ei = new EdgeInterface(m_vertices[id_vert1], m_vertices[id_vert2]); */
        /* m_interface->m_main_box.add_child(ei->m_top_edge); */
        /* m_edges[idx] = Edge(weight, ei); */
/// Le new EdgeInterface ne nécessite pas de delete car on a un shared_ptr
/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// mais il faut bien enlever le conteneur d'interface m_top_edge de l'arc de la main_box du graphe
        m_interface->m_main_box.remove_child( remed.m_interface->m_top_box );
    }

/// Le Edge ne nécessite pas non plus de delete car on n'a pas fait de new (sémantique par valeur)
/// Il suffit donc de supprimer l'entrée de la map pour supprimer à la fois l'Edge et le EdgeInterface
/// mais malheureusement ceci n'enlevait pas automatiquement l'interface top_edge en tant que child de main_box !
    m_vertices.erase( eidx );
/// Tester la cohérence : nombre d'arc entrants et sortants des sommets 1 et 2
    /*std::cout << m_vertices[remed.m_from].m_in.size() << " " << m_vertices[remed.m_from].m_out.size() << std::endl;
    std::cout << m_vertices[remed.m_to].m_in.size() << " " << m_vertices[remed.m_to].m_out.size() << std::endl;
    std::cout << m_edges.size() << std::endl;*/

//    for(auto& elem : m_vertices)
//    {
//        vertices[i]= elem.second;
//        i++;
//    }
//
//    i=0;
//    m_vertices.clear();
//
//    for(auto& elem : vertices)
//    {
//        m_vertices[i] = elem.second;
//        i++;
//    }

    //Rétrecir matrice d'adjacence
    m_adjacence.pop_back();
    for(auto& elem : m_adjacence)
        elem.pop_back();
    affichage_adjacence();
}

int* Graph::UneComposanteFortementConnexe(int s)
{
    //Variables locales
    int *c1, *c2;
    int *c;
    int* marques;
    int x,y;
    int ajoute=1;
    int ordre = m_vertices.size();

    //Allocation dynamique
    c1 = new int[ordre];
    c2 = new int[ordre];
    c = new int[ordre];
    marques = new int[ordre];

    //Initialiser les tableaux a 0
    for(int i=0; i<ordre; i++)
    {
        c1[i] = 0;
        c2[i] = 0;
        c[i] = 0;
        marques[i] = 0;
    }

    //Rendre le sommet s connexe
    c1[s] = 1;
    c2[s] = 1;

    //Recherche des composantes connexes partant de s à ajouter dans c1
    while(ajoute)
    {
        ajoute = 0;
        for(x=0; x<ordre; x++)
        {
            if(!marques[x] && c1[x])
            {
                marques[x] = 1;
                for(y=0; y<ordre; y++)
                {
                    if(m_adjacence[x][y] && !marques[y])
                    {
                        c1[y] = 1;
                        ajoute = 1;
                    }
                }
            }
        }
    }

    for(int i=0; i<ordre; i++)
        marques[i] = 0;
    ajoute = 1;

    //Recherche des composantes connexes arrivant à s à ajouter dans c2:
    while(ajoute)
    {
        ajoute = 0;
        for(x=0; x<ordre; x++)
        {
            if(!marques[x] && c2[x])
            {
                marques[x] = 1;
                for(y=0; y<ordre; y++)
                {
                    if(m_adjacence[y][x] && !marques[y])
                    {
                        c2[y] = 1;
                        ajoute = 1;
                    }
                }
            }
        }
    }

    //Composante fortement connexe c = intersection de c1 et c2
    for(x=0; x<ordre; x++)
        c[x] = c1[x] & c2[x];

    //Libérer la mémoire
    delete c1;
    delete c2;
    delete marques;

    //return
    return c;
}

void Graph::ToutesLesComposantesFortementsConnexes()
{
    //Variables locales
    int *marques;
    int x, y, nbc = 0;
    int ordre = m_vertices.size();
    bool n;
    int color = COULEURALEATOIRE;

    //Allocation dynamique
    m_connexite = new int*[ordre];
    for(int i=0; i<ordre; i++)
        m_connexite[i] = new int[ordre];
    marques = new int[ordre];

    //Initialiser les tableaux à 0
    for(int i=0; i<ordre; i++)
    {
        marques[i] =0;
        for(int j=0; j<ordre; j++)
            m_connexite[i][j] = 0;
    }

    //Recherche de la composante fortement connexe c pour tout les sommets non marqués
    for(x=0; x<ordre; x++)
    {
        if(!marques[x])
        {
            m_connexite[x] = UneComposanteFortementConnexe(x);
            marques[x] = 1;
            for(y=0; y<ordre; y++)
            {
                if(m_connexite[x][y] && !marques[y])
                    marques[y] = 1;
            }
        }
    }

    //Affichage
    std::cout << std::endl << "Composantes fortement connexes :" << std::endl;
    for(int i=0; i<ordre; i++)
    {
        for(int j=0; j<ordre; j++)
        {
            if(m_connexite[i][j])
            {
                m_vertices[j].m_interface->m_top_box.set_bg_color(color);
                std::cout << j << " ";
                n=true;
            }
        }
        if(n)
        {
            std::cout << std::endl;
            n=false;
            nbc++;
            color = COULEURALEATOIRE;
        }
    }
    std::cout << nbc << " composantes fortement connexes" << std::endl;

    //liberer la memoire
    delete marques;
    for(int i=0; i<m_vertices.size(); i++)
        delete m_connexite[i];
    delete[] m_connexite;
}
