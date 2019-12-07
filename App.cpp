//
// Created by cht on 2019/11/26.
//
#include "App.h"
#include "consolecolor.h"
#include <sstream>
#include <string>
#include "Tool.h"
#include "Log.h"
#include <iostream>

using namespace std;

void App::onLoad() {
    cout << consoleforecolor::violet << "��ӭ�����û���¼ϵͳ(AVLʵ��) VERSION:beta 5" << consoleforecolor::normal << endl;
}

string App::onTip() {
    string page = getContext().getPage();
    string subpage = getContext().subpage;
    if(subpage.empty()){
        cout << consoleforecolor::indigo << "page = " << page << endl;
        cout << getContext().getCurrentMenu();
    }

    string tip;

    if (subpage.empty()){
        tip = "��ѡ��˵��е�һ��";
    } else if(page == "main"){
        if(subpage == "import")
            tip = "����������Ҫ������ļ���(quitȡ��)";
        else if(subpage == "export")
            tip = "����������Ҫ�������ļ���(quitȡ��)";
    } else if(page == "admin"){
        if(subpage == "register")
            tip = "��������������˺ŵ��û���������(quitȡ��)";
        else if(subpage == "login")
            tip = "����������鿴�˺ŵ��û���(quitȡ��)";
        else if(subpage == "changepw")
            tip = "�����������޸��˺ŵ��û�����������(quitȡ��)";
        else if(subpage == "delete")
            tip = "����������ɾ���˺ŵ��û���(quitȡ��)";
        else if(subpage == "clear")
            tip = "��ȷ������ɾ�����л�������(y/n)";
    } else if(page == "client"){
        if (subpage == "register")
            tip = "����������ע���˺ŵ��û���������(quitȡ��)";
        else if (subpage == "login")
            tip = "�������û���������(quitȡ��)";
    } else if(page == "logged"){
        if (subpage == "changepw"){
            tip = "������������(quitȡ��)";
        } else if (subpage == "delete"){
            tip = "��ȷ������ɾ�����˺���(y/n)";
        }
    }

    return tip;
}

bool App::onHandleInput(string command) {
    string page = getContext().getPage();
    string subpage = getContext().subpage;
    if(getContext().isMenuIndexOf("����/��ӡ", command)){
        getContext().loginData.lPrintSlicing(toSimple,6);
    } else if(getContext().isMenuIndexOf("����/��ʾ������Ϣ", command)){
        getContext().loginData.print();
    } else if(page == "main"){
        if(subpage.empty()){
            if(getContext().isMenuIndexOf("�˳�����", command)) {
                return false;
            } else if(getContext().isMenuIndexOf("����", command)) {
                getContext().subpage = "import";
            } else if(getContext().isMenuIndexOf("����", command)) {
                getContext().subpage = "export";
            } else if(getContext().isMenuIndexOf("ת����ͨ�û�����", command)){
                getContext().setPage("client");
            } else if(getContext().isMenuIndexOf("ת������Ա����", command)){
                getContext().setPage("admin");
            } else {
                Log::e("�����������������");
            }
        } else if(subpage == "import" || subpage == "export"){
            int check = checkFileName(command);
            if(check == 1){
                Log::e("�ļ�������Ϊ��");
            } else if(check == 2){
                Log::e("�ļ���̫������Ӧ�ó���40���ַ�");
            } else if(check == 3){
                Log::e("�ļ�����Ӧ�ú���[/\\:*\"<>|?]��Щ�Ƿ��ַ�");
            } else {
                if(subpage == "import"){
                    if(command == "quit"){
                        getContext().subpage = "";
                    } else {
                        if(getContext().loginData.loadFile(command)){
                            Log::d("����ɹ�");
                        } else {
                            Log::e("�ļ�������");
                        }
                        getContext().subpage = "";
                    }

                } else if(subpage == "export"){
                    if(command == "quit"){
                        getContext().subpage = "";
                    } else {
                        getContext().loginData.saveFile(command);
                        Log::d("�����ɹ�");
                        getContext().subpage = "";
                    }
                }
            }
        }
    } else if(page == "client"){
        if (subpage.empty()){
            if (getContext().isMenuIndexOf("�˻�������", command)){
                getContext().setPage("main");
            } else if (getContext().isMenuIndexOf("ע��", command)){
                getContext().subpage = "register";
            } else if(getContext().isMenuIndexOf("��¼", command)){
                getContext().subpage = "login";
            }
        } else if(subpage == "register" || subpage == "login") { //�����˺Ż��¼�˺�
            LoginData data;
            if(command == "quit"){
                getContext().subpage = "";
            } else {

                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("�û������Ȳ��Ϸ���Ӧ��2-20λ");
                    } else if(result == 2){
                        Log::e("�û������ַǷ��ַ�");
                    } else if(result == 3){
                        Log::e("���볤�Ȳ��Ϸ���Ӧ��6-64λ");
                    } else if(result == 4) {
                        Log::e("������ַǷ��ַ�");
                    }
                } else {
                    if(subpage == "register"){
                        if(getContext().loginData.find(data)!= nullptr){
                            Log::e("�Ѵ��ڸ��û����޷������û�");
                            getContext().subpage = "";
                        } else {
                            getContext().loginData.update(data);
                            Log::d("�����û��ɹ�");
                            getContext().subpage = "";
                        }
                    } else {
                        bnode<LoginData>* u = getContext().loginData.find(data);
                        if(u == nullptr){
                            Log::e("�û�������");
                            getContext().subpage = "";
                        } else if(u->value.password != data.password){
                            Log::e("�������");
                        } else {
                            Log::d(data.username + "��¼�ɹ�");
                            getContext().current = data.username;
                            getContext().setPage("logged");
                            getContext().subpage = "";
                        }
                    }
                }
            }
        }
    } else if(page == "logged"){
        if( subpage.empty()){
            if (getContext().isMenuIndexOf("�˳���¼", command)){
                getContext().setPage("client");
            } else if (getContext().isMenuIndexOf("��ӭ", command)){
                Log::w("Welcome!" + getContext().current);
            } else if(getContext().isMenuIndexOf("�޸�����",command)) {
                getContext().subpage = "changepw";
            } else if(getContext().isMenuIndexOf("ɾ���˺�", command)) {
                getContext().subpage = "delete";
            }

        } else if(subpage == "delete"){
            if(command == "quit" || command == "n"){
                getContext().subpage = "";
            } else if (command == "y"){
                LoginData data(getContext().current, "");
                int result = getContext().loginData.remove(data);
                Log::d("ɾ���û��ɹ�");
                getContext().setPage("client");
                getContext().subpage = "";
            } else {
                Log::e("�����������������");
            }
        } else if(subpage == "changepw"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                LoginData data(getContext().current, command);
                bnode<LoginData>* u = getContext().loginData.find(data);
                if(u->value.password == data.password) {
                    Log::e("�����ظ�");
                } else {
                    getContext().loginData.update(data);
                    Log::d("�޸�����" + data.username + "�ɹ�");
                    getContext().subpage = "";
                }
            }
        }
    } else if(page == "admin"){
        if( subpage.empty()){
            if (getContext().isMenuIndexOf("�˻�������", command)){
                getContext().setPage("main");
            } else if(getContext().isMenuIndexOf("����˺�", command)){
                getContext().subpage = "register";
            } else if(getContext().isMenuIndexOf("�鿴�˺���Ϣ", command)){
                getContext().subpage = "login";
            } else if(getContext().isMenuIndexOf("�޸�����", command)){
                getContext().subpage = "changepw";
            } else if(getContext().isMenuIndexOf("ɾ���˺�", command)){
                getContext().subpage = "delete";
            } else if(getContext().isMenuIndexOf("�������", command)){
                getContext().subpage = "clear";
            }
        } else if(subpage == "register" || subpage == "login") { //�����˺Ż��¼�˺�
            LoginData data;
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                if(subpage == "login"){
                    command += " 123456";
                }

                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("�û������Ȳ��Ϸ���Ӧ��2-20λ");
                    } else if(result == 2){
                        Log::e("�û������ַǷ��ַ�");
                    } else if(result == 3){
                        Log::e("���볤�Ȳ��Ϸ���Ӧ��6-64λ");
                    } else if(result == 4) {
                        Log::e("������ַǷ��ַ�");
                    }
                } else {
                    if(subpage == "register"){
                        if(getContext().loginData.find(data)!= nullptr){
                            Log::e("�Ѵ��ڸ��û����޷������û�");
                            getContext().subpage = "";
                        } else {
                            getContext().loginData.update(data);
                            Log::d("�����û�" + data.username + "�ɹ�");
                            getContext().subpage = "";
                        }
                    } else {
                        bnode<LoginData>* u = getContext().loginData.find(data);
                        if(u == nullptr){
                            Log::e("�û�������");
                            getContext().subpage = "";
                        } else {
                            cout << u->value << endl;
                            getContext().subpage = "";
                        }
                    }
                }
            }
        } else if(subpage == "delete"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                int check = checkUsername(command);
                if(check == 1){
                    Log::e("�û������Ȳ��Ϸ���Ӧ��2-20λ");
                } else if(check == 2){
                    Log::e("�û������ַǷ��ַ�");
                } else {
                    LoginData data(command, "");
                    int result = getContext().loginData.remove(data);
                    if(!result){
                        Log::e("�û�������");
                    } else {
                        Log::d("ɾ���û��ɹ�");
                    }
                    getContext().subpage = "";
                }

            }
        } else if(subpage == "changepw"){
            if(command == "quit"){
                getContext().subpage = "";
            } else {
                LoginData data;
                int result = LoginData::tryParse(command,data);

                if(result != 0){
                    if(result == 1){
                        Log::e("�û������Ȳ��Ϸ���Ӧ��2-20λ");
                    } else if(result == 2){
                        Log::e("�û������ַǷ��ַ�");
                    } else if(result == 3){
                        Log::e("���볤�Ȳ��Ϸ���Ӧ��6-64λ");
                    } else if(result == 4) {
                        Log::e("������ַǷ��ַ�");
                    }
                } else {
                    bnode<LoginData>* u = getContext().loginData.find(data);
                    if(u == nullptr){
                        Log::e("�û�������");
                        getContext().subpage = "";
                    } else {
                        getContext().loginData.update(data);
                        Log::d("�޸�����" + data.username + "�ɹ�");
                        getContext().subpage = "";
                    }
                }
            }
        } else if(subpage == "clear"){
            if(command == "quit" || command == "n"){
                getContext().subpage = "";
            } else if (command == "y"){
                getContext().loginData.clear();
                Log::d("��������л���");
                getContext().subpage = "";
            } else {
                Log::e("�����������������");
            }
        }
    }
    return true;
}